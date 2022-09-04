/*

	ofxWinMenu basic example - ofApp.h

	Copyright (C) 2016-2017 Lynn Jarvis.

	http://www.spout.zeal.co

	=========================================================================
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
	=========================================================================
*/

#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h" // Menu addon
#include <vector>
#include <stack>
#include <queue>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// for min heap
#define MAX_ELEMENTS 5000  // ���� �� �ڷ��� ������ �����Ѵ�.
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1) // ���� �� �������� �Ǵ��ϴ� ��ũ���̴�.
#define HEAP_EMPTY(n) (!n) // ���� ��������� �Ǵ��ϴ� ��ũ���̴�.

class ofApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();

	void keyPressed(int key); // Traps escape key if exit disabled
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void freeMemory();
	bool DFS(int,int,int,int);
	void dfsdraw(vector<pair<int, int>>);

	int HEIGHT;//�̷��� ����
	int WIDTH;//�̷��� �ʺ�
	int** visited;//�湮���θ� ������ ������
	int isOpen; //������ ���ȴ����� �Ǵ��ϴ� ����. 0�̸� �ȿ��Ȱ� 1�̸� ���ȴ�.

	// Menu
	ofxWinMenu * menu; // Menu object
	void appMenuFunction(string title, bool bChecked); // Menu return function

	// Used by example app
	ofTrueTypeFont myFont;
	ofImage myImage;
	float windowWidth, windowHeight;
	HWND hWnd; // Application window
	HWND hWndForeground; // current foreground window

	// Example menu variables
	bool bShowInfo;
	bool bFullscreen;
	bool bTopmost;
	bool isdfs;
	// Example functions
	void doFullScreen(bool bFull);
	void doTopmost(bool bTop);

	/* �̷� ���� ����*/
	vector<string> maze;

	/* ������ �迭 Ž���� ���� ������*/
	/* �Ʒ�, ������, ��, ���� ������ Ž�� ����*/
	vector<int> xCheck = { 0,1,0,-1 };
	vector<int> yCheck = { 1,0,-1,0 };
	vector<int> xMove = { 0, 2, 0, -2 };
	vector<int> yMove = { 2, 0, -2, 0 };

	/* for Dfs*/
	stack<pair<int, int>> PathDfs; // ��ΰ� ����Ǵ� ����
	vector<pair<int, int>> finalPathDfs; // ��ΰ� ����Ǵ� ����. PathDfs ������ �״�� �����ؿ´�.


	/* Exit handling*/
	bool isexit; // ���α׷��� ���� ���θ� �����ϴ� ����

	/* ���� ���� */
	int N; // �̷��� ���� ������
	int M; // �̷��� ���� ������

	int *prevMAZE; // �̷��� �ٷ� ���� ���� ������ �����ϱ� ���� ����
	int *nowMAZE; // �̷��� ���� ���� ������ �����ϱ� ���� ����

	int *TYPES; // ������ ������ �����ϴ� �迭
	int *COUNT; // �� ������ ������ �����ϴ� �迭
	int *INDEX; // �� ������ TYPES �迭������ �ε����� �����ϴ� �迭
	int *CONNECTED; // �� ������ �������� ����Ǿ� �ִ� ���� ���θ� �����ϴ� �迭

	int connected; // ���� �˰����� ������ �� ���� ���� �� ������ �����ϴ� ����
	int types; // ������ ������ ����
	int roomNum; // ���ȣ�� 1���� �����Ѵ�.

	void makeMaze(); // �̷� ����� �Լ�
	void horizontalCheck(int); // ���򿬰������ ó���ϴ� �Լ�
	void verticalCheck(int); // ������������� ó���ϴ� �Լ�



	/* newFunction */
	bool getFile(); // 20181288.maz �о���� �Լ�
	bool canMove(int, int); // �̵� ������ ��ġ���� �Ǵ����ִ� �Լ�
	void init(); // ������ �����ϴ� �� �ʿ��� �������� �ʱ�ȭ �ϴ� �Լ�
	void drawMaze(); // �̷� �׸���
	void drawScoreTime(); // ������ �ð��� �׸���
	void drawHint(); // ��Ʈ�� �׸���
	void drawPlayer(); //�÷��̾� �׸���
	void drawEndPoint(); // ������������ �׸���
	void drawHintComment(); // ��Ʈ ��Ʈ �׸���

	void kruskal(); // ũ�罺Į �˰����� �̿��Ͽ� ��Ʈ�� �׷��� ��θ� ���� �Լ�
	void kruskalDraw(); // ũ�罺Į �˰����� �̿��Ͽ� ���� �ּ� ���� Ʈ���� ȭ�鿡 �׷��ִ� �Լ�


	FILE *fp; // �ؽ�Ʈ ������ �����ϱ� ���� ���� ������

	/* new variable*/
	int playerX; // �÷��̾��� x��ǥ
	int playerY; // �÷��̾��� y��ǥ
	vector<pair<int, int>> endPoints; //  ������ ��ǥ��
	vector<pair<int, int>> checkPoints; //  ������ ��ǥ��

	bool rightPressed; // ������ ����Ű�� ���ȴ� �� ���θ� �����ϴ� ����
	bool leftPressed; // ���� ����Ű�� ���ȴ� �� ���θ� �����ϴ� ����
	bool upPressed; // ���� ����Ű�� ���ȴ� �� ���θ� �����ϴ� ����
	bool downPressed; //// �Ʒ��� ����Ű�� ���ȴ� �� ���θ� �����ϴ� ����

	bool isHint; // ��Ʈ�� �־�� �ϴ� ��Ȳ������ ���θ� �����ϴ� ����

	int score; // ���� ������ �����ϴ� ����
	int highScore = -1; // �ְ� ������ �����ϴ� ����
	
	int timeLeft; // ���� �ð��� �����ϴ� ����
	int timeChecker; // ȭ�鿡 ǥ���ϴ� �ð��� 1�ʸ��� ��ȭ�ϰ� �ϱ� ���� ���� ����

	int FPS; // �ʴ� ������ ���� �����ϴ� ����
	int hintTimer; // 2�ʵ��� ��Ʈ�� �����ֱ� ���� �ʿ��� ����

	int ballCount = 4; // ���� �������� 4���� �����Ͽ���.

	char screenVariable = 's'; // ���α׷��� �����Ű�� �ʱ�ȭ���� �������� �Ѵ�.

	vector<int> ranking_vector; // ��ŷ ������ �����ϴ� �迭

	/* Node for min heap */
	// ������ ������ ���µ�, �̴� head�� tail�� ���������μ� ������ �� �ִ�.
	// weight�� ������ ���̸� �����Ѵ�.
	// path�� ���� ��θ� �����Ѵ�.
	typedef struct {
		int head; // ������ ������
		int tail; // ������ ����
		int weight; // ������ ����� cost
		vector<pair<int, int>> path; // ���� ���
	} element;

	element heap[MAX_ELEMENTS]; // ũ�罺Į �˰����� ������ ��, ������ ������ ���̴�.
	element heapNode; // ���� �� ���
	element deletedNode; // delete method�� �̿��Ͽ� ������ �ֻ�� ��带 �����ϴ� �����̴�.
	int heapCount; // ���� ���� ���ִ� ������ ������ �����Ѵ�.
	int edgeCount; // ���� PATHS ���Ϳ� ������ ������ ������ �����Ѵ�.
	// End of Node for min heap

	/* Union find algorithm */
	int *parent; // unoin-find �˰����� �̿��ϱ� ���� ���� �θ� �迭�̴�.

	vector<vector<pair<int, int>>> PATHS; // ũ�罺Į �˰����� �̿��Ͽ� �� �ּ� ���� Ʈ���� ��θ� �����ϴ� �����̴�.


	void insert_heap(element, int*); //�� �ڷᱸ���� ��带 �����ϴ� �Լ��̴�.
	void delete_heap(int*); // �� �ڷᱸ������ ��带 �����ϴ� �Լ��̴�.

	int collapsingFind(int); // tree ���·� ������ ���� ��, ��Ʈ�� �� ȿ�������� ã�� �� �ְ� ���ִ� �Լ��̴�.
	void weightedUnion(int, int); // �� ���� ������ ��ĥ ��, �������� Ʈ���� ������ �� �ֵ��� ���ִ� �Լ��̴�.

	void registerScore(); // ���� ������ ranking_vector�� �߰��Ѵ�. �׻� �������� ������ �Ǿ��ֵ��� �Ѵ�.
	void Vector2File(); // ranking_vector�� ����Ǿ� �ִ� �������� scores.txt�� �߰��Ѵ�.
	void File2Vector(); // score.txt�� �ִ� ������ ���α׷� ���� ������ ranking_vector�� �ҷ��´�.

	void drawScoreList(); // ����ȭ�鿡 top3�� ǥ�����ش�.
};