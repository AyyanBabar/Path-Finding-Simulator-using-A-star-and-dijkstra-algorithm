#define OLC_PGE_APPLICATION
#include<iostream>
#include"olcConsoleGameEngine.h"
#include"need.h"
#include<fstream>
#include<math.h>
using namespace std;
bool dijkstra = 0;
class Project :public olcConsoleGameEngine {
	LIST_2D<Spot> L;
	int xblockes;
	int yblockes;
	bool bupdate;
	Spot* start;
	Spot* end;
	PQueue<Spot*> open;

	PQueue<Spot*> close;

public:
	Project() {
		m_sAppName = L"Kaitou";
	}
protected:
	bool OnUserCreate() override {
		bupdate = false;
		start = NULL;
		end = NULL;
		xblockes = 15;
		yblockes = 30;
		for (int i = 0; i < yblockes; i++) {
			for (int j = 0; j < xblockes; j++) {
				Spot s(i, j);
				L.add(s, i);
			}
		}
		for (int i = 0; i < yblockes; i++) {
			for (int j = 0; j < xblockes; j++) {
				if (i > 0) {
					L[i][j].data.nab(&L[i - 1][j].data);//add top
					if (j > 0)
						L[i][j].data.nab(&L[i - 1][j - 1].data);//top left
					if (j < xblockes - 1)
						L[i][j].data.nab(&L[i - 1][j + 1].data);//top right
				}
				if (i < yblockes - 1) {
					L[i][j].data.nab(&L[i + 1][j].data);//add bottom
					if (j > 0)
						L[i][j].data.nab(&L[i + 1][j - 1].data);//bottom left
					if (j < xblockes - 1)
						L[i][j].data.nab(&L[i + 1][j + 1].data);//bottom right
				}
				if (j > 0) {
					L[i][j].data.nab(&L[i][j - 1].data);//add left
				}
				if (j < xblockes - 1) {
					L[i][j].data.nab(&L[i][j + 1].data);//add right
				}
			}
		}
		return true;
	}
	void refresh() {
		for (int i = 0; i < yblockes; i++) {
			for (int j = 0; j < xblockes; j++) {
				Spot* s = &L[i][j].data;
				s->g = INF;
				s->f = INF;
				s->prev = NULL;
				s->c = FG_BLUE;
			}
		}
		while (!open.isEmpty()) {
			open.dequeue(false);
		}
		while (!close.isEmpty()) {
			close.dequeue(false);
		}
		if (start) {
			start->g = 0;
			start->f = heuristic(start, end);
		}
	}
	double heuristic(Spot* a, Spot* b) {
		if (dijkstra)
			return 0;
		int x = a->x - b->x;
		int y = a->y - b->y;
		double dis = ((double)x * (double)x) + ((double)y * (double)y);
		return sqrt(dis);
	}
	double visualHuristic(Spot* a, Spot* b) {
		double dis = abs(a->x - b->x) + abs(a->y - b->y);
		return dis;
	}

	void  A_star() {
		if (bupdate) {
			if (open.isEmpty()) {
				refresh();
				bupdate = false;
				return;
			}
			Spot* s;
			s = open.peek();
			open.update();
			s = open.peek();
			if (s == end) {
				end->backtrack();
				bupdate = false;
				return;
			}
			open.size();
			open.dequeue();
			close.enqueue(s, false);
			for (int i = 0; i < s->nnabs; i++) {
				Spot* k = s->Nabs[i];
				if (!k->iswall && !close.isIn(k))
				{
					float tempg = 0;
					if ((s->x == k->x - 1 && s->y == k->y - 1) || (s->x == k->x - 1 && s->y == k->y + 1) || (s->x == k->x + 1 && s->y == k->y - 1) || (s->x == k->x + 1 && s->y == k->y + 1))
						tempg = s->g + 1.1;
					else
						tempg = s->g + 1;
					if (!open.isIn(k))
						open.enqueue(k);
					else if (tempg >= k->g)
						continue;
					k->g = tempg;
					k->f = k->g + heuristic(k, end);
					k->vh = visualHuristic(k, end);
					k->prev = s;

					open.update();
				}
			}
		}
	}
	void exporting() {
		gotoxy(0, 0);
		ofstream out;
		cout << "Enter: ";
		string str;
		cin >> str;
		str = str + ".txt";
		out.open(str);
		if (out) {
			for (int i = 0; i < yblockes; i++) {
				for (int j = 0; j < xblockes; j++) {
					Spot* s = &L[i][j].data;
					if (s == end)
						out << 3;
					else if (s == start)
						out << 2;
					else if (s->iswall)
						out << 1;
					else
						out << 0;
				}
			}
		}
		out.close();
	}
	bool importing() {
		gotoxy(0, 0);
		ifstream in;
		cout << "Enter: ";
		string str;
		cin >> str;
		str = str + ".txt";
		in.open(str);
		if (in) {
			char temp;
			for (int i = 0; i < yblockes; i++) {
				for (int j = 0; j < xblockes; j++) {
					Spot* s = &L[i][j].data;
					in >> temp;
					if (temp == '3')
						end = s;
					else if (temp == '2')
						start = s;
					else if (temp == '1')
						s->iswall = true;
					else
						s->iswall = false;
				}
			}
			in.close();
			return true;
		}
		else
			return false;
	}
	bool OnUserUpdate(float fElapsedTime) override {
		int nsize = 5;
		int nborder = 1;
		int node_x = m_mousePosX / nsize;
		int node_y = m_mousePosY / nsize;
		if (m_keys[VK_ESCAPE].bHeld) {
			return false;
		}
		if (m_keys['E'].bHeld) {
			exporting();
			return true;
		}
		if (m_keys['I'].bHeld) {
			if (importing()) {
				refresh();
				if (start)
					open.enqueue(start);
				if (start && end)
					bupdate = true;
			}
			return true;
		}
		if (m_mouse[0].bReleased)
		{
			if (node_x >= 0 && node_x < yblockes) {
				if (node_y >= 0 && node_y < xblockes)
				{
					if (m_keys[VK_SHIFT].bHeld) {
						this->start = &L[node_x][node_y].data;
						if (end) {
							refresh();
							open.enqueue(start);
							bupdate = true;
						}
					}
					else if (m_keys[VK_CONTROL].bHeld) {
						this->end = &L[node_x][node_y].data;
						if (end->iswall)
							end->iswall = !end->iswall;
						if (start) {
							refresh();
							open.enqueue(start);
							bupdate = true;
						}
					}
					else {
						L[node_x][node_y].data.iswall = !L[node_x][node_y].data.iswall;
						if (&L[node_x][node_y].data == end)
							end = NULL;
						if (end && start) {
							refresh();
							open.enqueue(start);
							bupdate = true;
						}
					}
				}
			}
			else {
				if (node_y < xblockes) {
					dijkstra = !dijkstra;
					refresh();
					if (start)
						open.enqueue(start);
					if (start && end)
						bupdate = true;
				}
			}
		}
		Fill(0, 0, ScreenWidth(), ScreenHeight(), ' ');
		for (int i = 0; i < yblockes; i++) {
			for (int j = 0; j < xblockes; j++) {
				if (L[i][j].data.g == INF) {
					Fill(i * nsize + nborder, j * nsize + nborder, (i + 1) * nsize - nborder, (j + 1) * nsize - nborder, PIXEL_HALF, L[i][j].data.iswall ? FG_BLUE : FG_WHITE);
				}
				else
					Fill(i * nsize + nborder, j * nsize + nborder, (i + 1) * nsize - nborder, (j + 1) * nsize - nborder, PIXEL_SOLID, L[i][j].data.c);

				if (&L[i][j].data == start)
					Fill(i * nsize + nborder, j * nsize + nborder, (i + 1) * nsize - nborder, (j + 1) * nsize - nborder, PIXEL_SOLID, FG_GREEN);
				else if (&L[i][j].data == end)
					Fill(i * nsize + nborder, j * nsize + nborder, (i + 1) * nsize - nborder, (j + 1) * nsize - nborder, PIXEL_SOLID, FG_RED);
			}
		}
		if (dijkstra)
			DrawString(yblockes * nsize, 7 * nsize, L"Dijkstra");
		else
			DrawString(yblockes * nsize, 7 * nsize, L"A*");
		A_star();

		return true;
	}
	bool OnUserDestroy() {
		while (!this->close.isEmpty()) {
			close.dequeue();
		}
		while (!this->open.isEmpty()) {
			open.dequeue();
		}

		return true;
	}
};
int main() {
	Project demo;
	demo.ConstructConsole(160, 75, 8, 8);
	demo.Start();
	system("PAUSE");
	return 0;
}