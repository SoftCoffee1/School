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
		bool readFile();
		void freeMemory();
		bool DFS();
		void dfsdraw();
		bool BFS(); // BFS --> 3주차 과제
		void bfsdraw(); //bfsdraw --> 3주차 과제
		int HEIGHT;//미로의 높이
		int WIDTH;//미로의 너비
		char** input;//텍스트 파일의 모든 정보를 담는 이차원 배열이다.
		int** visited;//방문여부를 저장할 포인터
		int maze_col;//미로칸의 열의 인덱스를 가리킨다.
		int maze_row;//미로칸의 행의 인덱스를 가리킨다.
		int k;
		int isOpen; //파일이 열렸는지를 판단하는 변수. 0이면 안열렸고 1이면 열렸다.
		int isDFS;//DFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
		int isBFS;//BFS함수를 실행시켰는지 판단하는 변수. 0이면 실행안했고 1이면 실행했다.
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
		vector<vector<int>> checkDFS; // DFS를 이용하여 탐색한 모든 경로를 중복없이 저장한다.

		/* for BFS*/
		bool isbfs; // bfs 탐색이 진행되었는지 여부를 저장하는 변수
		int*** Parent; // 부모노드를 저장하기 위한 배열
		queue<pair<int, int>> PathBfs; // bfs탐색을 한 이후 얻어낸 최적의 경로
		vector<vector<int>> checkBFS; // bfs탐색을 한 이후 탐색과정에서 지나간 모든 경로


		/* Exit handling*/
		bool isexit;
};
