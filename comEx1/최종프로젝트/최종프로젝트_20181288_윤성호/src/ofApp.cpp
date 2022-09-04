// 1. 랭크 시스템 구현
// 2. 빡겜수치 --> 

// 1. 전체적인 코드 정리
// 2. ppt
// 3. 보고서
//      -> 엔터는 시작화면에서만 작동하도록 구현
//      -> hint : 크루스칼 알고리즘을 이용한 최소 신장 트리로 구현
//      -> rank : 랭킹 시스템 구현
// 4. 영상

/*

	ofxWinMenu basic example - ofApp.cpp

	Example of using ofxWinMenu addon to create a menu for a Microsoft Windows application.

	Copyright (C) 2016-2017 Lynn Jarvis.

	https://github.com/leadedge

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

	03.11.16 - minor comment cleanup
	21.02.17 - rebuild for OF 0.9.8

*/
#include "ofApp.h"
#include <iostream>
using namespace std;
//--------------------------------------------------------------
void ofApp::setup() {

	ofSetWindowTitle("Maze Escape"); // Set the app name on the title bar
	ofSetFrameRate(FPS);
	ofBackground(200);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isOpen = 0;  // 파일이 열렸는지 여부를 판단하는 변수
	isexit = false; // 파일이 닫혔는지 여부를 판단하는 변수.
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);

	// Load a font rather than the default
	myFont.load("verdana.ttf", 12, true, true);

	// Load an image for the example
	//myImage.loadImage("lighthouse.jpg");

	// Window handle used for topmost function
	hWnd = WindowFromDC(wglGetCurrentDC());

	// Disable escape key exit so we can exit fullscreen with Escape (see keyPressed)
	ofSetEscapeQuitsApp(false);

	//
	// Create a menu using ofxWinMenu
	//

	// A new menu object with a pointer to this class
	menu = new ofxWinMenu(this, hWnd);

	// Register an ofApp function that is called when a menu item is selected.
	// The function can be called anything but must exist. 
	// See the example "appMenuFunction".
	menu->CreateMenuFunction(&ofApp::appMenuFunction);

	// Create a window menu
	HMENU hMenu = menu->CreateWindowMenu();

	//
	// Create a "File" popup menu
	//
	HMENU hPopup = menu->AddPopupMenu(hMenu, "File");

	//
	// Add popup items to the File menu
	//

	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	
	// Set the menu to the window
	menu->SetWindowMenu();

	File2Vector(); // score.txt의 내용을 vector에 옮겨준다.


} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	//
	// File menu
	//
	if (title == "Exit") {
		isexit = true;
		Vector2File();
		freeMemory();
		ofExit(); // Quit the application
	}


} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

	switch (screenVariable) {

	// 시작화면일 때 실행되는 부분
	case 's':
		break;

	// 게임하는 중일 경우 실행되는 부분
	case 'g':

		
		// 만약 빨간 점 중 하나를 먹었다면, 빨간점을 vector에서 지워준다.
		for (unsigned int i = 0; i < endPoints.size(); i++) {

			if (abs(playerX - 10*endPoints[i].second) < 10 && abs(playerY - 10*endPoints[i].first) < 10) {
				endPoints.erase(endPoints.begin() + i);
			}
		}

		// 만약 모든 빨간 점을 먹었다면, 
		// 현재 미로사이즈의 절반만큼과 남은 시간의 절반의 합 만큼의 시간을 추가로 부여하고,
		// 새로운 미로를 생성하여 다음 스테이지로 넘어간다.
		if (endPoints.size() == 0) {
			timeLeft += N/2 + timeLeft / 2; // 현재 미로사이즈의 절반만큼과 남은 시간의 절반의 합만큼의 시간을 추가로 부여한다.
			makeMaze();
			getFile();
			isHint = false;
		}

		// 오른쪽 방향키가 눌려있고, 오른쪽으로 이동이 가능한 경우 이동시킨다. 
		if (rightPressed && canMove(playerX + 2, playerY)) {
			playerX += 2;
		}

		// 왼쪽 방향키가 눌려있고, 왼쪽으로 이동이 가능한 경우 이동시킨다.
		if (leftPressed && canMove(playerX - 2, playerY)) {
			playerX -= 2;
		}

		// 위쪽 방향키가 눌려있고, 위쪽으로 이동이 가능한 경우 이동시킨다.
		if (upPressed && canMove(playerX, playerY - 2)) {
			playerY -= 2;
		}

		// 아래쪽 방향키가 눌려있고, 아래쪽으로 이동이 가능한 경우 이동시킨다.
		if (downPressed && canMove(playerX, playerY + 2)) {
			playerY += 2;
		}

		// 게임을 시작한 즉시 계속 실행되는 부분이다.
		// 점수를 매 frame마다 1 증가시켜주고, 시간은 1초마다 1씩 줄어들게 설정했다.
		if (isOpen) {
			score += 1;
			timeChecker += 1;

			// timeChecker의 값이 FPS와 같다는 말은 1초가 흘렀다는 것을 의미한다.
			if (timeChecker == FPS) {
				timeLeft -= 1; // 남은시간을 1 감소시킨다.
				timeChecker = 0; // timeCheck를 0으로 설정한다.
			}
		}

		// 스페이스바를 눌렀을 시 실행되는 부분이다.
		// 스페이스바를 누르면 hintTimer는 2*FPS로 설정된다. 이를 매 프레임마다 1 감소시키면 2초후에 0이 된다.
		if (isHint) {
			hintTimer -= 1;

			if (hintTimer == 0) {
				cout << "hint finished!" << endl;
				isHint = false;
				ofBackground(200);
				
			}
		}

		// 시간이 종료되었다면 게임 종료
		if (timeLeft == 0) {
			freeMemory();
			screenVariable = 's';

			if (highScore < score) highScore = score;
			registerScore();

			cout << "Final score: " << score << endl;
		}
		break;

	}

}


//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0, 0, 255);
	ofSetLineWidth(5);


	switch (screenVariable) {

	// 시작화면을 그려준다.
	case 's':
		char gameNameStr[256];
		sprintf(gameNameStr, "ENTER TO START");
		myFont.drawString(gameNameStr, ofGetWidth() / 2 - 100, ofGetHeight()/2 - 100);

		drawScoreList();


		break;

	// 게임화면을 그려준다.
	case 'g':
		drawMaze();// 미로를 그려준다.
		drawScoreTime(); // 점수와 시간을 그려준다.
		if (isHint) drawHint(); // 스페이스바가 눌렸다면 정답경로를 2초동안 화면에 그려준다.
		drawPlayer(); // 플레이어를 그려준다.
		drawEndPoint(); // 도착점을 그려준다.
		drawHintComment(); // 힌트 문구를 그려준다.

		break;
	}


} // end Draw

// 미로를 그려주는 함수
void ofApp::drawMaze() {

	
	// HEIGHT만큼 움직이며
	for (int i = 0; i < HEIGHT; i++) {

		// WIDTH만큼 움직이며
		for (int j = 0; j < WIDTH; j++) {

			// 만약 가로벽을 만나면
			if (maze[i][j] == '-') {
				ofDrawLine(10 * (j - 1) - 2, 10 * i, 10 * (j + 1) + 2, 10 * i); // 벽을 기준으로 양옆으로 그림을 그려준다.
			}

			// 만약 세로벽을 만나면
			else if (maze[i][j] == '|') {
				ofDrawLine(10 * j, 10 * (i - 1) - 2, 10 * j, 10 * (i + 1) + 2); // 벽을 기준으로 위아래로 그림을 그려준다.
			}

		}
	}
}

// 점수와 시간을 그려주는 함수
void ofApp::drawScoreTime() {

	char scoreStr[256];
	char highScoreStr[256];
	char timeLeftStr[256];

	if (isOpen) {
		if (highScore != -1) {
			sprintf(scoreStr, "High Score : %d ", highScore);
			myFont.drawString(scoreStr, ofGetWidth() - 200, 20);
		}
		sprintf(scoreStr, "Now Score : %d ", score);
		myFont.drawString(scoreStr, ofGetWidth() - 200, 50);
		sprintf(timeLeftStr, "Time Left : %d ", timeLeft);
		myFont.drawString(timeLeftStr, ofGetWidth() - 200, 80);
	}
}

// 스페이스바를 누르면 힌트경로를 그려주는 함수
void ofApp::drawHint() {

	ofSetColor((2 * FPS - hintTimer) * 2);
	if (isOpen)
	{
		kruskalDraw(); // 최소 신장 트리를 그려준다.
	}
	else
		cout << "you must open file first" << endl;
}

// 플레이어를 그려주는 함수
void ofApp::drawPlayer() {
	ofSetColor(0, 0, 255);
	ofDrawCircle(playerX, playerY, 5);
}

// 도착점을 그려주는 함수
void ofApp::drawEndPoint() {
	ofSetColor(255, 0, 0);

	for (unsigned int i = 0; i < endPoints.size(); i++) {

		int endY = endPoints[i].first;
		int endX = endPoints[i].second;
		ofDrawCircle(10*endX, 10*endY, 5);
	}
}

// 스페이스바를 누르면 힌트를 볼 수 있다는 멘트를 화면에 그려주는 함수
void ofApp::drawHintComment(){

	ofSetColor(0);
	char hintComment[256];
	sprintf(hintComment, "Press SPACE BAR for Hint ");
	myFont.drawString(hintComment, 30, 10 * HEIGHT + 30);
}

void ofApp::drawScoreList() {

	char first[256];
	char second[256];
	char third[256];

	
	// 1등이 존재하면 화면에 출력
	if (ranking_vector.size() >= 1) {
		sprintf(first, "first place : %d ", ranking_vector[0]);
		myFont.drawString(first, ofGetWidth() - 200, 100);
	}

	// 2등이 존재하면 화면에 출력
	if (ranking_vector.size() >= 2) {
		sprintf(second, "second place : %d ", ranking_vector[1]);
		myFont.drawString(second, ofGetWidth() - 200, 150);
	}

	// 3등이 존재하면 화면에 출력
	if (ranking_vector.size() >= 3) {
		sprintf(third, "third place : %d ", ranking_vector[2]);
		myFont.drawString(third, ofGetWidth() - 200, 200);
	}

	return;
}



void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if (bFull) {
		// Remove the menu but don't destroy it
		menu->RemoveWindowMenu();
		// hide the cursor
		ofHideCursor();
		// Set full screen
		ofSetFullscreen(true);
	}
	else {
		// return from full screen
		ofSetFullscreen(false);
		// Restore the menu
		menu->SetWindowMenu();
		// Restore the window size allowing for the menu
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		// Centre on the screen
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if (bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if (bTop) {
		// get the current top window for return
		hWndForeground = GetForegroundWindow();
		// Set this window topmost
		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
		// Reset the window that was topmost before
		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

	// Escape key exit has been disabled but it can be checked here
	if (key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if (bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	if (key == 'f') {
		bFullscreen = !bFullscreen;
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

	// 오른쪽을 누른 경우 변수 변경
	if (key == OF_KEY_RIGHT) {
		rightPressed = true;
	}

	// 왼쪽을 누른 경우 변수 변경
	if (key == OF_KEY_LEFT) {
		leftPressed = true;
	}

	// 위쪽을 누른 경우 변수 변경
	if (key == OF_KEY_UP) {
		upPressed = true;
	}
		
	// 아래쪽을 누른 경우 변수 변경
	if (key == OF_KEY_DOWN) {
		downPressed = true;
	}

	// 미로탈출 힌트를 위한 작업                    
	if (key == ' ') {
		isHint = true; // 힌트가 보여지는 상황임을 저장하는 변수
		hintTimer = 2 * FPS; // 힌트는 2초간 보여진다.

		edgeCount = 0; // 간선이 노드개수 - 1개가 될때 까지 크루스칼 알고리즘을 돌리기 위해 만든 변수
		heapCount = 0; // 현재 힙에 들어있는 원소의 개수를 저장하는 변수이다.

		// union-find 알고리즘을 위한 부모 노드 저장 배열
		parent = (int*)malloc(ballCount * sizeof(int));

		if (parent == NULL) {
			cout << "front failed" << endl;
			return;
		}
		for (int i = 0; i < ballCount; i++) parent[i] = -1; // union-find algorithm을 위해 -1로 초기화한다.

		// 크루스칼 알고리즘을 실행한다.
		kruskal();
	}
	

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

	if ((key == OF_KEY_RETURN) && (screenVariable == 's')) {
		init(); // 게임시작을 위한 초기화과정을 진행해준다.
	}

	// 오른쪽을 뗀 경우 변수 변경
	if (key == OF_KEY_RIGHT) {
		rightPressed = false;
	}

	// 왼쪽을 뗀 경우 변수 변경
	if (key == OF_KEY_LEFT) {
		leftPressed = false;
	}

	// 위쪽을 뗀 경우 변수 변경
	if (key == OF_KEY_UP) {
		upPressed = false;
	}

	// 아래쪽을 뗀 경우 변수 변경
	if (key == OF_KEY_DOWN) {
		downPressed = false;
	}

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}


// 게임 시작을 위한 초기화 과정이다.
void ofApp::init() {

	N = 4; // 미로의 가로 : 4
	M = 4; // 미로의 세로 : 4
	makeMaze(); // 미로파일을 생성한다.
	getFile(); // 만들어진 파일에서 미로를 불러온다.

	// 아무런 방향키도 눌려있지 않은 상태이다.
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;

	// 힌트는 시작부터 보여주지 않는다.
	isHint = false;

	score = 0; // 점수는 0점부터 시작한다.

	timeLeft = 12; // 남은 시간을 설정한다. 12초를 준다.
	timeChecker = 0; // 1초마다 화면상에 1씩 줄어드는 인터페이스를 만들기위한 변수이다.

	FPS = 50; // 초당 50 프레임이 뜨도록 설정한다.
	hintTimer = 0; // 힌트가 화면에 잠시동안만 보이도록 하기 위한 변수이다.

	screenVariable = 'g'; // 게임화면으로 전환한다.
}

// 만들어진 미로파일로부터 미로를 불러와서 vector 자료구조에 저장한다.
bool ofApp::getFile() {

	// 파일을 불러온다.
	ofFile file("20181288.maz");

	if (!file.exists()) {
		cout << "Target file does not exists." << endl;
		return false;
	}
	else {
		cout << "We found the target file." << endl;
		isOpen = 1;
	}

	// 파일객체를 생성한다.
	ofBuffer buffer(file);


	// 미로는 maze라는 이름의 vector 자료구조에 담는다.
	// 담기전에 초기화를 해준다.
	maze.clear();
	vector<string>().swap(maze);


	// buffer에 저장된 파일 정보를 라인별로 받아오며
	for (auto line : buffer.getLines()) {
		maze.push_back(line); // string 타입의 각 라인의 내용을 maze 벡터에 추가한다.
	}

	HEIGHT = maze.size(); // 미로의 HEIGHT는 maze 벡터의 사이즈이다.
	WIDTH = maze[0].size(); // 미로의 WIDTH는 maze[0] 문자열의 사이즈이다.

	// visited 이차원 배열 동적할당
	visited = new int*[HEIGHT]; // 이차원 배열을 동적할당 해주기 위해 new를 이용한다.
	for (int i = 0; i < HEIGHT; i++) visited[i] = new int[WIDTH]; // 각각의 visited[i]를 new를 이용하여 동적할당해준다.


	return true; // 파일 읽기가 성공했기 때문에 true를 반환한다.
}

// 메모리를 해제해준다.
void ofApp::freeMemory() {

	// 게임을 종료한 경우
	if (isexit) {
		// visited 배열의 메모리를 해제해주는 부분
		for (int x = 0; x < HEIGHT; x++) {
			delete[] visited[x]; // 이차원배열이 구현된 부분의 메모리를 해제해주고
		}
		delete[] visited; // 일차원배열이 구현된 부분의 메모리를 해제해준다.
	}
}

// DFS 탐색을 하는 함수
bool ofApp::DFS(int startY, int startX, int endY, int endX)
{
	// visited 배열을 모두 0으로 초기화한다.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0은 아직 방문하지 않았다는 것을 의미하고, 1은 방문했다는 것을 의미한다.
	}

	// 최종경로를 저장할 vector를 초기화한다.
	finalPathDfs.clear();
	vector<pair<int, int>>().swap(finalPathDfs);

	PathDfs.push({ startY, startX }); // 미로의 출발은 (startY, startX)에서 시작한다.
	visited[startY][startX] = 1; // (startY,startX)은 방문했기 때문에 방문처리를 해준다.

	// PathDfs가 빈 스택이 아니면 while loop를 실행한다.
	while (!PathDfs.empty()) {

		// 끝 지점에 도달하면 while loop를 빠져나온다.
		if (PathDfs.top().first == endY  && PathDfs.top().second == endX) break;

		int nowY = PathDfs.top().first; // 스택의 최상단에 저장된 위치의 Y좌표
		int nowX = PathDfs.top().second; // 스택의 최상단에 저장된 위치의 X좌표

		int notMoved = 1; // 스택의 최상단에 저장된 위치의 인접한 4방향에 대해 이동가능하면 0, 이동가능하지 않으면 1로 설정한다.

		// 스택의 최상단에 저장된 위치의 인접한 4방향을 순회하며
		for (int i = 0; i < 4; i++) {

			int checkY = nowY + yCheck[i]; // 이동방향에 벽의 존재여부를 확인하기 위한 위치의 Y좌표
			int checkX = nowX + xCheck[i]; // 이동방향에 벽의 존재여부를 확인하기 위한 위치의 X좌표

			int nextY = nowY + yMove[i]; // 이동할 예정인 위치의 Y좌표
			int nextX = nowX + xMove[i]; // 이동할 예정인 위치의 X좌표

			// 만약 이동할 예정인 위치가 미로를 벗어난다면 다음 위치를 탐색하러 간다.
			if (!((0 < nextY) && (nextY < HEIGHT) && (0 < nextX) && (nextX < WIDTH))) continue;

			// 이동방향에 벽이 없고, 아직 방문하지 않은 위치이면 다음으로 이동이 가능하다.
			if (maze[checkY][checkX] == ' ' && visited[nextY][nextX] == 0) {
				PathDfs.push({ nextY, nextX }); // 다음 위치를 PathDfs에 push해준다.
				visited[nextY][nextX] = 1; // 다음 위치를 방문처리해준다.
				notMoved = 0; // 인접한 방향으로 이도이 가능하므로 0으로 설정한다.
				break;
			}
		}

		// 인접한 4방향 중 어디로도 이동이 불가능하면 PathDfs에서 pop을 진행한다.
		if (notMoved) PathDfs.pop();
	}

	// PathDfs에 저장된 좌표를 finalPathDfs라는 벡터에 저장한다. 인덱스로 접근하기 위함이다.
	while (!PathDfs.empty()) {
		int y = PathDfs.top().first; // 최상단에 저장된 위치의 y좌표이다. 
		int x = PathDfs.top().second; // 최상단에 저장된 위치의 x좌표이다.

		PathDfs.pop(); // 최상단 원소를 pop해준다.

		finalPathDfs.push_back({ y, x }); // 최상단 위치의 y,x좌표를 pair로 finalPathDfs 벡터에 뒤에 추가해준다.
	}

	return 1; // 함수를 종료한다.
}

// 미로의 실제 탈출 경로를 그린다.
void ofApp::dfsdraw(vector<pair<int, int>> PATH)
{
	for (unsigned int i = 0; i < PATH.size() - 1; i++) {
		int pY1 = PATH[i].first; // PATH에 저장된 한 위치의 y좌표
		int pX1 = PATH[i].second; // PATH에 저장된 한 위치의 x좌표
		int pY2 = PATH[i + 1].first; //  PATH에 저장된 다른 위치의 y좌표
		int pX2 = PATH[i + 1].second; // PATH에 저장된 다른 위치의 x좌표

		ofDrawLine(10 * pX1, 10 * pY1, 10 * pX2, 10 * pY2); // 저장된 두 위치를 양 끝으로 하여 선분을 그린다.
	}


}


// 미로를 생성한다.
void ofApp::makeMaze() {

	srand((unsigned int)time(NULL)); // 무작위로 연결여부를 선택하기 위해 시드넘버를 변화시켜주는 부분

	N++; // 미로의 가로 사이즈를 1 증가시킨다.
	M++; // 미로의 세로 사이즈를 1 증카시킨다.
	playerX = 10; // 플레이어의 x좌표
	playerY = 10; // 플레이어의 y좌표


	// 도착점 좌표를 담는 vector를 초기화한다.
	endPoints.clear();
	vector<pair<int, int>>().swap(endPoints);

	int count = 0; // 생성된 도착점의 개수를 저장한다.

	// ballcount만큼 도착점이 생성되었을 때 loop를 빠져나온다.
	// 도착점은 서로 중복되지 않으며, 출발점에 생성되지 않는다.
	while (count < ballCount) {
		int endY = rand() % N + 1;
		int endX = rand() % M + 1;

		// 현재 무작위로 생성한 도착점의 좌표를 추가할 지 말지 판단하느 변수이다.
		bool dontAdd = false;

		// 모든 도착점은 서로 중복되지 않으며, 출발점에 생성되지 않는다.
		for (unsigned int i = 0; i < endPoints.size(); i++) {
			if ((endPoints[i].first == endY && endPoints[i].second == endX) || (endY == 1 && endX == 1)) {
				dontAdd = true;
				break;
			}
		}

		// 도착점이 중복되거나, 출발점에 생성이 되었다면 도착점을 다시 생성한다.
		if (dontAdd) continue;

		// 성공적으로 도착점이 생성되었다면, endPoints 벡터에 삽입한다.
		endPoints.push_back({ 2 * endY - 1 , 2 * endX - 1 });

		// 도착점이 추가되었으므로 count를 1 증가시킨다.
		count++;


	}
	roomNum = 1; // 방 번호는 1부터 배정한다.
	types = 1; // 종류의 개수를 저장한다.

	fp = fopen("./data/20181288.maz", "w"); // 20181288.maz 파일을 생성하고, 이미 있다면 새롭게 다시 쓴다.

	if (fp == NULL) {
		printf("File open Error"); // 파일이 존재하지 않는다면 에러메세지를 출력한다.
		return; // 프로그램을 종료한다.
	}

	else {
		printf("파일이 생성되었습니다.\n"); // 파일이 성공적으로 생성되었다면 성공했다는 메세지를 출력한다.
	}

	prevMAZE = (int*)calloc(N, sizeof(int*)); // 동적할당으로 미로 행 배열 정의, 0으로 초기화.
	nowMAZE = (int*)calloc(N, sizeof(int*)); // 동적할당으로 미로 행 배열 정의, 0으로 초기화.

	// 가장 꼭대기 지점은 모두 벽으로 가로막혀있다.
	for (int i = 0; i <= 2 * N; i++) {

		// 짝수지점은 모서리이므로 +를 출력한다.
		if (i % 2 == 0) {
			//printf("+");
			fprintf(fp, "+");
		}

		// 홀수지점은 모서리가 아니므로 벽(-)을 출력한다.
		else {
			//printf("-");
			fprintf(fp, "-");
		}
	}

	// 다음 칸에 미로를 출력해주기 위해 newline을 출력해준다.
	//printf("\n");
	fprintf(fp, "\n");


	/* 수평 과정 */
	for (int line = 0; line < M; line++) {

		horizontalCheck(line); // 현재 확인하고 있는 행에 대해 수평 연결을 처리해준다.


		TYPES = (int*)malloc(types * sizeof(int)); // 집합의 종류를 저장할 배열
		COUNT = (int*)calloc(types, sizeof(int)); // 집합의 종류의 개수를 저장할 배열
		INDEX = (int*)malloc((N*M + 1) * sizeof(int)); // 동적할당으로 인덱스를 저장할 배열 정의.

		for (int i = 1; i <= N * M; i++) INDEX[i] = -1; // 집합의 인덱스는 -1로 초기화해준다.

		int tIndex = 0; // TYPES에 값을 저장할 때 필요한 변수
		for (int i = 0; i < N; i++) {

			// 아직 종류를 등록하지 않았다면
			if (INDEX[nowMAZE[i]] == -1) {
				INDEX[nowMAZE[i]] = tIndex; // INDEX 배열에 값을 저장한다.
				TYPES[tIndex++] = nowMAZE[i]; // TYPES 배열에 현재 집합의 번호를 저장한다.
			}

			COUNT[INDEX[nowMAZE[i]]]++; // COUNT 배열의 값을 1 증가시킨다.
		}

		for (int i = 0; i < N; i++) {
			prevMAZE[i] = nowMAZE[i]; // 이전 행으로 옮겨주고
			nowMAZE[i] = 0; // 현재 행은 초기화해준다.
		}

		// 다음칸에 미로를 출력해주어야하기 때문에 newline을 출력해준다.
		//printf("\n");
		fprintf(fp, "\n");


		/* 수직 과정 */
		verticalCheck(line); // 수직연결을 처리해준다.

		// 다음줄에 미로를 출력해주기 위해 newline을 출력해준다.
		//printf("\n");
		fprintf(fp, "\n");

	}

	// 파일을 닫아준다.
	fclose(fp);
}


// 수평과정을 처리하는 함수
void ofApp::horizontalCheck(int line) {

	// 집합번호가 배정이 되어 있지 않은 칸은 전역변수로 저장된 roomNum을 이용해 매번 다른 수로 번호를 매겨준다.
	for (int i = 0; i < N; i++) {
		if (nowMAZE[i] == 0) nowMAZE[i] = roomNum++;
	}

	types = 1; // 현재 행에서 탐색하는 집합의 개수를 1로 초기화해준다.

	// 행을 모두 살펴보며
	for (int i = 0; i <= N; i++) {

		// 시작 지점은 항상 벽이다. 벽의 모양은 "|" 이다.
		if (i == 0) {
			//printf("| ");
			fprintf(fp, "| ");
			continue; // for loop의 처음부분으로 돌아간다.
		}

		// 끝 지점은 항상 벽이다. 벽의 모양은 "|" 이다.
		if (i == N) {
			//printf("|");
			fprintf(fp, "|");
			break; // for loop를 탈출한다.
		}

		// 만약 마지막 줄이라면
		if (line == M - 1) {

			// 인접한 칸의 집합번호가 다르다면 경로가 없다는 것을 의미하므로 무조건 연결을 해주어야 한다.
			if (nowMAZE[i - 1] != nowMAZE[i]) {

				int nowMin = min(nowMAZE[i - 1], nowMAZE[i]); // 인접한 두 칸의 번호 중 작은 번호
				int nowMax = max(nowMAZE[i - 1], nowMAZE[i]); // 인접합 두 칸의 번호 중 큰 번호

				// 집합 연결시켜주기. 행 전체를 돌며 nowMax와 같은 집합번호를 가지고 있는 칸은 nowMin으로 변경해준다.
				for (int same = 0; same < N; same++)
					if (nowMAZE[same] == nowMax) nowMAZE[same] = nowMin;

				// 방이 연결되었기 때문에 공백을 출력해준다.
				//printf("  ");
				fprintf(fp, "  ");
			}

			// 방이 연결되어있지 않다면 벽을 출력해준다. 벽은 "|"모양이다.
			else {
				//printf("| ");
				fprintf(fp, "| ");
			}
		}


		// 만약 현재 확인하는 행이 마지막 줄이 아니라면
		else if (line < M - 1) {
			connected = rand() % 2; // 연결 할지 말지 무작위로 선택한다.

			// 아직 연결이 되어 있지 않으면서 연결을 시켜주려고 할 때 -> 이미 연결이 되어있으면 연결시켜주면 안된다.
			if (connected == 1 && (nowMAZE[i - 1] != nowMAZE[i])) {

				int nowMin = min(nowMAZE[i - 1], nowMAZE[i]); // 인접한 두 칸의 번호 중 작은 번호
				int nowMax = max(nowMAZE[i - 1], nowMAZE[i]); // 인접한 두 칸의 번호 중 큰 번호

				// 집합 연결시켜주기. 행 전체를 돌며 nowMax와 같은 집합번호를 가지고 있는 칸은 nowMin으로 변경해준다.
				for (int same = 0; same < N; same++)
					if (nowMAZE[same] == nowMax) nowMAZE[same] = nowMin;

				// 방이 연결되어 있으므로 공백을 출력해준다.
				//printf("  ");
				fprintf(fp, "  ");
			}

			// 방이 연결되어있지 않거나, 연결을 하지 않는 것으로 결정을 했다면
			else {
				// 벽을 출력한다. 벽은 "|" 모양이다.
				//printf("| ");
				fprintf(fp, "| ");
				types++; // 집합의 개수를 하나 증가시켜준다.
			}
		}
	}
}

// 수직과정을 처리하는 함수
void ofApp::verticalCheck(int line) {

	// 마지막 행을 탐색하고 있다면
	if (line == M - 1) {
		for (int i = 0; i <= 2 * N; i++) {

			// 짝수지점은 모서리이므로 +를 출력한다.
			if (i % 2 == 0) {
				//printf("+");
				fprintf(fp, "+");
			}

			// 홀수지점은 모서리가 아니므로 벽(-)을 출력한다.
			else {
				//printf("-");
				fprintf(fp, "-");
			}
		}

		// 마지막 행의 처리까지 끝났으므로 함수를 종료한다.
		return;
	}

	// 아래행과 집합의 연결여부를 저장하는 배열이다. 0으로 초기화한다.
	CONNECTED = (int*)calloc(types, sizeof(int));

	for (int i = 0; i <= 2 * N; i++) {

		// 짝수지점은 모서리이므로 +를 출력한다.
		if (i % 2 == 0) {
			//printf("+");
			fprintf(fp, "+");
		}

		// 홀수지점은 벽일수도 아닐 수 도 있다.
		else {

			// 현재 집합이 이전 행에 하나밖에 없는데 아직 연결이 되어있지 않다면 무조건 연결을 시켜주어야한다.
			if (COUNT[INDEX[prevMAZE[i / 2]]] == 1 && CONNECTED[INDEX[prevMAZE[i / 2]]] == 0) {
				nowMAZE[i / 2] = prevMAZE[i / 2]; // 이전 행의 집합번호를 현재 행의 집합번호가 되게 한다.
				CONNECTED[INDEX[prevMAZE[i / 2]]] = 1; // 현재 집합은 아래 행과 연결되어있으므로 1로 변경해준다.
				COUNT[INDEX[prevMAZE[i / 2]]]--; // 현재 탐색 중인 집합번호의 개수는 1 줄여준다.
				//printf(" "); // 연결이 되어 있으므로 공백을 출력해준다.
				fprintf(fp, " ");
			}

			// 현재 집합이 이전 행에 하나 이상 있으면 연결을 해줄 수도 있고 해주지 않을 수도 있다.
			else if (COUNT[INDEX[prevMAZE[i / 2]]] >= 1) {
				connected = rand() % 2;

				// 연결을 해준다고 결정을 했다면
				if (connected == 1) {
					nowMAZE[i / 2] = prevMAZE[i / 2]; // 이전 행의 집합 번호가 현재 행의 집합번호가 되게 한다.
					CONNECTED[INDEX[prevMAZE[i / 2]]] = 1; // 현재 집합은 아래 행과 연결되어있으므로 1로 변경해준다.
					//printf(" "); // 연결되었기 때문에 공백을 출력해준다.
					fprintf(fp, " ");
				}

				// 연결을 하지 않는다고 결정을 했다면
				else {
					//printf("-"); // 벽을 그려준다. 벽은 "-" 모양이다.
					fprintf(fp, "-");
				}

				// 현재 탐색하는 집합번호의 개수를 1 줄여준다.
				COUNT[INDEX[prevMAZE[i / 2]]]--;
			}
		}
	}
}


// 방향키로 이동할 수 있는 지 여부를 판단하는 함수
bool ofApp::canMove(int x, int y) {

	int i, j;

	// HEIGHT만큼 움직이며
	for (i = 0; i < HEIGHT; i++) {

		// WIDTH만큼 움직이며
		for (j = 0; j < WIDTH; j++) {

			// 만약 가로벽을 만나면
			if (maze[i][j] == '-') {

				// 플레이어가 벽을 만나면 움직일 수 없다.
				if ((x >= 10 * (j - 1) - 1) && (x <= 10 * (j + 1) + 1) && (y == 10 * i)) return false;
			}

			// 만약 세로벽을 만나면
			else if (maze[i][j] == '|') {
				// 플레이어가 벽을 만나면 움직일 수 없다.
				if ((y > 10 * (i - 1) - 1) && (y < 10 * (i + 1) + 1) && (x == 10 * j)) return false;

			}
		}
	}

	// 이동가능하므로 true를 리턴해준다.
	return true;
}



// 최소 신장 트리를 생성하기 위한 크루스칼 알고리즘을 구현한 함수
void ofApp::kruskal() {

	// 최소 신장 트리의 간선을 저장할 PATHS 벡터를 초기화한다.
	PATHS.clear();
	vector<vector<pair<int, int>>>().swap(PATHS);

	// 현재 최소 신장 트리가 포함할 노드들을 담을 checkPoints 벡터를 초기화한다.
	checkPoints.clear();
	vector<pair<int, int>>().swap(checkPoints);

	// checkPoints는 endPoints의 모든 원소들을 담고 있다.
	checkPoints = endPoints;

	// 현재 플레이어의 위치 또한 checkPoints 벡터에 담는다.
	int nowPlayerY = playerY / 10;
	int nowPlayerX = playerX / 10;

	if ((nowPlayerY % 2) == 0) nowPlayerY++;
	if ((nowPlayerX % 2) == 0) nowPlayerX++;

	checkPoints.push_back({ nowPlayerY, nowPlayerX });

	heapCount = 0; // 힙에 들어가있는 원소는 0으로 초기화한다.


	// 모든 체크포인트에 대해 DFS를 수행하여 각각을 연결하는 간선의 cost를 계산한다.
	for (unsigned int i = 0; i < checkPoints.size(); i++) {

		for (unsigned int j =i+1; j < checkPoints.size(); j++) {

			// 현재 선택한 두개의 점에 대해 DFS 경로를 찾아서 갈 수 있는 cost를 계산한다.
			DFS(checkPoints[i].first, checkPoints[i].second, checkPoints[j].first, checkPoints[j].second);

			int p1 = i; // 현재 시작점
			int p2 = j; // 현재 끝점
			int weight = finalPathDfs.size(); // 현재 두 점 사이를 연결하는 간선의 cost
			vector<pair<int, int>> path = finalPathDfs; // 현재 두 점 사이를 연결하는 실제 경로

			/* min heap 구성하기*/
			heapNode.head = p1; // 현재 시작점
			heapNode.tail = p2; // 현재 끝점
			heapNode.weight = -weight; //현재 두 점 사이를 연결하는 간선의 cost -> min heap을 구성하기 위한 테크닉
			heapNode.path = path; // 현재 두 점 사이를 연결하는 실제 경로
			insert_heap(heapNode, &heapCount); // 만들어진 노드를 heap에 삽입한다.
		}
	}

	// heap의 원소의 개수가 0이 아니고, 현재 MST에 연결된 edge개수가 num_of_vertices-1이 아닌 경우
	while (heapCount != 0 && edgeCount != ballCount ) {
		delete_heap(&heapCount); // 힙의 최상단 노드를 지운다. 

		int headRoot = collapsingFind(deletedNode.head); // find 함수를 이용해 현재 시작부분의 루트를 구한다.
		int tailRoot = collapsingFind(deletedNode.tail); // find 함수를 이용해 현재 끝 부분의 루트를 구한다.

		// cycle이 발생하는 edge라면 무시하고 건너뛴다.
		if (headRoot == tailRoot) continue;

		// 집합을 union 해준다.
		weightedUnion(headRoot, tailRoot);
		edgeCount++; // 간선이 추가가 되었으므로 1 증가시킨다.

		// 현재 선택한 간선을 PATHS 벡터에 추가해준다.
		PATHS.push_back(deletedNode.path);

	}

}

// 크루스칼 알고리즘을 생성한 각각의 간선을 그려준다.
void ofApp::kruskalDraw() {

	// 최소 신장 트리를 구성하는 모든 경로를 그려준다.
	for (unsigned int i = 0; i < PATHS.size(); i++) {
		dfsdraw(PATHS[i]);
	}
}



// min heap에 원소를 삽입하는 함수
void ofApp::insert_heap(element item, int *n) {

	int i;

	// HEAP_FULL error handling
	if (HEAP_FULL(*n)) {
		printf("Heap is full.\n");
		return;
	}

	i = ++(*n);

	// 루트가 아니고, 현재 cost가 부모의 cost보다 큰 경우 (절댓값이 작은 경우)에 부모와 교체해준다.
	while ((i != 1) && (item.weight > heap[i / 2].weight)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

// min heap에서 원소를 삭제한다.
void ofApp::delete_heap(int *n) {

	int parent, child;
	element item, temp;

	if (HEAP_EMPTY(*n)) {
		printf("Heap is empty.\n");
		return;
	}

	// 최상단 노드를 뽑아온다.
	item = heap[1];

	temp = heap[(*n)--];
	parent = 1;
	child = 2;

	// 힙 자료구조를 유지하며 현재 최상단 노드에 위치한 노드를 아래로 내려보낸다.
	while (child <= *n) {
		if ((child < *n) && (heap[child].weight < heap[child + 1].weight)) child++;
		if (temp.weight >= heap[child].weight) break;

		heap[parent] = heap[child];
		parent = child;
		child *= 2;
	}

	heap[parent] = temp;
	deletedNode = item;
	return;
}

// 경로압축기법이 적용된 find 함수
int ofApp::collapsingFind(int i) {
	int root, trail, lead;

	// 현재 루트를 찾는다.
	for (root = i; parent[root] >= 0; root = parent[root]);

	// 현재 선택된 노드부터 루트까지 올라가며 모두 parent값을 루트로 교체해준다.
	for (trail = i; trail != root; trail = lead) {
		lead = parent[trail];
		parent[trail] = root;
	}

	// 루트를 반환한다.
	return root;
}


// 균형잡힌 트리 구성을 도와주는 union함수
// find 과정을 훨씬 빠르게 수행할 수 있도록 해준다.
void ofApp::weightedUnion(int i, int j) {
	int temp = parent[i] + parent[j];
	
	// 원소의 개수가 큰 집합의 루트를 원소의 개수가 적은 집합의 루트의 부모 노드가 되도록 만든다.
	if (parent[i] > parent[j]) {
		parent[i] = j;
		parent[j] = temp;
	}

	else {
		parent[j] = i;
		parent[i] = temp;
	}
}


// 현재 점수를 ranking_vector에 내림차순으로 저장한다.
void ofApp::registerScore() {
	
	// 각각의 원소들을 순회하며 저장해야하는 위치를 찾으면 insert함수를 이용하여 벡터의 중간에 원소를 삽입해주었다.
	for (unsigned int i = 0; i < ranking_vector.size(); i++) {
		if (ranking_vector[i] < score) {
			ranking_vector.insert(ranking_vector.begin() + i, score);
			return;
		}
	}
	// 만약 삽입이 되지 않았다면 원소가 하나도 없었다는 것을 의미하므로 원소를 삽입해준다.
	ranking_vector.push_back(score);
	return;
}

// ranking_vector에 저장되어 있는 점수들을 scores.txt에 저장해준다.
void ofApp::Vector2File() {

	fp = fopen("scores.txt", "wt"); // 파일포인터를 이용하여 scores.txt파일을 쓰기 모드로 연다.

	// 파일을 여는데 실패했다면 에러 메세지를 출력해주고 함수를 종료한다.
	if (fp == NULL) {
		cout << "file open error" << endl;
		return;
	}

	// ranking_vector에 대해서 모든 원소들을 score.txt에 써준다.
	for (unsigned int i = 0; i < ranking_vector.size(); i++) {
		fprintf(fp, "%d ", ranking_vector[i]);
	}

	// 파일을 닫아준다.
	fclose(fp);
	return;
}

//  scores.txt에 저장되어 있는 점수들을 ranking_vector에 저장해준다.
void ofApp::File2Vector() {

	// scores.txt를 읽기 모드로 연다.
	fp = fopen("scores.txt", "rt");

	// 파일을 여는데 실패했다면 에러 메세지를 출력해주고 함수를 종료한다.
	if (fp == NULL) {
		cout << "file open error" << endl;
		return;
	}

	int fileItem; // score.txt 내부의 내용을 저장할 변수이다.

	// 파일의 끝까지 읽으며 ranking_vector에 점수를 삽입해준다.
	while (EOF != fscanf(fp, "%d", &fileItem)) {
		ranking_vector.push_back(fileItem);
	}

	if (ranking_vector.size() >= 1)
		highScore = ranking_vector[0]; // 최고 점수를 저장한다.

	// 파일을 닫아준다.
	fclose(fp);
	return;
}