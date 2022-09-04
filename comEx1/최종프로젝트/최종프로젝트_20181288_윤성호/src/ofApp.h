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
#define MAX_ELEMENTS 5000  // 힙에 들어갈 자료의 개수를 제한한다.
#define HEAP_FULL(n) (n == MAX_ELEMENTS - 1) // 힙이 꽉 차있음을 판단하는 매크로이다.
#define HEAP_EMPTY(n) (!n) // 힙이 비어있음을 판단하는 매크로이다.

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

	int HEIGHT;//미로의 높이
	int WIDTH;//미로의 너비
	int** visited;//방문여부를 저장할 포인터
	int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.

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

	/* 미로 저장 벡터*/
	vector<string> maze;

	/* 이차원 배열 탐색을 위한 단위들*/
	/* 아래, 오른쪽, 위, 왼쪽 순으로 탐색 진행*/
	vector<int> xCheck = { 0,1,0,-1 };
	vector<int> yCheck = { 1,0,-1,0 };
	vector<int> xMove = { 0, 2, 0, -2 };
	vector<int> yMove = { 2, 0, -2, 0 };

	/* for Dfs*/
	stack<pair<int, int>> PathDfs; // 경로가 저장되는 스택
	vector<pair<int, int>> finalPathDfs; // 경로가 저장되는 벡터. PathDfs 정보를 그대로 복사해온다.


	/* Exit handling*/
	bool isexit; // 프로그램의 종료 여부를 저장하는 변수

	/* 전역 변수 */
	int N; // 미로의 가로 사이즈
	int M; // 미로의 세로 사이즈

	int *prevMAZE; // 미로의 바로 직전 행의 정보를 저장하기 위해 선언
	int *nowMAZE; // 미로의 현재 행의 정보를 저장하기 위해 선언

	int *TYPES; // 집합의 종류를 저장하는 배열
	int *COUNT; // 각 집합의 개수를 저장하는 배열
	int *INDEX; // 각 집합의 TYPES 배열에서의 인덱스를 저장하는 배열
	int *CONNECTED; // 각 집합이 수직으로 연결되어 있는 것의 여부를 저장하는 배열

	int connected; // 엘러 알고리즘을 진행할 때 벽을 쌓을 지 말지를 결정하는 변수
	int types; // 종류의 개수를 저장
	int roomNum; // 방번호는 1부터 시작한다.

	void makeMaze(); // 미로 만드는 함수
	void horizontalCheck(int); // 수평연결과정을 처리하는 함수
	void verticalCheck(int); // 수직연결과정을 처리하는 함수



	/* newFunction */
	bool getFile(); // 20181288.maz 읽어오는 함수
	bool canMove(int, int); // 이동 가능한 위치인지 판단해주는 함수
	void init(); // 게임을 시작하는 데 필요한 변수들을 초기화 하는 함수
	void drawMaze(); // 미로 그리기
	void drawScoreTime(); // 점수와 시간을 그리기
	void drawHint(); // 힌트를 그리기
	void drawPlayer(); //플레이어 그리기
	void drawEndPoint(); // 도착지점들을 그리기
	void drawHintComment(); // 힌트 멘트 그리기

	void kruskal(); // 크루스칼 알고리즘을 이용하여 힌트로 그려줄 경로를 얻어내는 함수
	void kruskalDraw(); // 크루스칼 알고리즘을 이용하여 얻은 최소 신장 트리를 화면에 그려주는 함수


	FILE *fp; // 텍스트 파일을 생성하기 위한 파일 포인터

	/* new variable*/
	int playerX; // 플레이어의 x좌표
	int playerY; // 플래이어의 y좌표
	vector<pair<int, int>> endPoints; //  도착점 좌표들
	vector<pair<int, int>> checkPoints; //  도착점 좌표들

	bool rightPressed; // 오른쪽 방향키가 눌렸는 지 여부를 저장하는 변수
	bool leftPressed; // 왼쪽 방향키가 눌렸는 지 여부를 저장하는 변수
	bool upPressed; // 위쪽 방향키가 눌렸는 지 여부를 저장하는 변수
	bool downPressed; //// 아래쪽 방향키가 눌렸는 지 여부를 저장하는 변수

	bool isHint; // 힌트를 주어야 하는 상황인지의 여부를 저장하는 변수

	int score; // 최종 점수를 저장하는 변수
	int highScore = -1; // 최고 점수를 저장하는 변수
	
	int timeLeft; // 남은 시간을 저장하는 변수
	int timeChecker; // 화면에 표시하는 시간을 1초마다 변화하게 하기 위해 만든 변수

	int FPS; // 초당 프레임 수를 저장하는 변수
	int hintTimer; // 2초동안 힌트를 보여주기 위해 필요한 변수

	int ballCount = 4; // 현재 도착점은 4개로 설정하였다.

	char screenVariable = 's'; // 프로그램이 실행시키면 초기화면이 나오도록 한다.

	vector<int> ranking_vector; // 랭킹 정보를 저장하는 배열

	/* Node for min heap */
	// 힙에는 간선이 들어가는데, 이는 head와 tail을 저장함으로서 구현할 수 있다.
	// weight는 간선의 길이를 저장한다.
	// path는 실제 경로를 저장한다.
	typedef struct {
		int head; // 간선의 시작점
		int tail; // 간선의 끝점
		int weight; // 간선의 경로의 cost
		vector<pair<int, int>> path; // 실제 경로
	} element;

	element heap[MAX_ELEMENTS]; // 크루스칼 알고리즘을 진행할 때, 간선을 저장할 힙이다.
	element heapNode; // 힙에 들어갈 노드
	element deletedNode; // delete method를 이용하여 삭제한 최상단 노드를 저장하는 변수이다.
	int heapCount; // 현재 힙에 들어가있는 간선의 개수를 저장한다.
	int edgeCount; // 현재 PATHS 벡터에 포함한 간선의 개수를 저장한다.
	// End of Node for min heap

	/* Union find algorithm */
	int *parent; // unoin-find 알고리즘을 이용하기 위해 만든 부모 배열이다.

	vector<vector<pair<int, int>>> PATHS; // 크루스칼 알고리즘을 이요하여 얻어낸 최소 신장 트리의 경로를 저장하는 벡터이다.


	void insert_heap(element, int*); //힙 자료구조에 노드를 삽입하는 함수이다.
	void delete_heap(int*); // 힙 자료구조에서 노드를 삭제하는 함수이다.

	int collapsingFind(int); // tree 형태로 집합을 만들 때, 루트를 더 효율적으로 찾을 수 있게 해주는 함수이다.
	void weightedUnion(int, int); // 두 개의 집합을 합칠 때, 균형잡힌 트리를 구성할 수 있도록 해주는 함수이다.

	void registerScore(); // 현재 점수를 ranking_vector에 추가한다. 항상 오름차순 정렬이 되어있도록 한다.
	void Vector2File(); // ranking_vector에 저장되어 있는 점수들을 scores.txt에 추가한다.
	void File2Vector(); // score.txt에 있는 내용을 프로그램 내부 벡터인 ranking_vector로 불러온다.

	void drawScoreList(); // 시작화면에 top3를 표시해준다.
};