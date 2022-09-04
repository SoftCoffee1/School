// 1. ��ũ �ý��� ����
// 2. ���׼�ġ --> 

// 1. ��ü���� �ڵ� ����
// 2. ppt
// 3. ����
//      -> ���ʹ� ����ȭ�鿡���� �۵��ϵ��� ����
//      -> hint : ũ�罺Į �˰����� �̿��� �ּ� ���� Ʈ���� ����
//      -> rank : ��ŷ �ý��� ����
// 4. ����

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
	isOpen = 0;  // ������ ���ȴ��� ���θ� �Ǵ��ϴ� ����
	isexit = false; // ������ �������� ���θ� �Ǵ��ϴ� ����.
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

	File2Vector(); // score.txt�� ������ vector�� �Ű��ش�.


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

	// ����ȭ���� �� ����Ǵ� �κ�
	case 's':
		break;

	// �����ϴ� ���� ��� ����Ǵ� �κ�
	case 'g':

		
		// ���� ���� �� �� �ϳ��� �Ծ��ٸ�, �������� vector���� �����ش�.
		for (unsigned int i = 0; i < endPoints.size(); i++) {

			if (abs(playerX - 10*endPoints[i].second) < 10 && abs(playerY - 10*endPoints[i].first) < 10) {
				endPoints.erase(endPoints.begin() + i);
			}
		}

		// ���� ��� ���� ���� �Ծ��ٸ�, 
		// ���� �̷λ������� ���ݸ�ŭ�� ���� �ð��� ������ �� ��ŭ�� �ð��� �߰��� �ο��ϰ�,
		// ���ο� �̷θ� �����Ͽ� ���� ���������� �Ѿ��.
		if (endPoints.size() == 0) {
			timeLeft += N/2 + timeLeft / 2; // ���� �̷λ������� ���ݸ�ŭ�� ���� �ð��� ������ �ո�ŭ�� �ð��� �߰��� �ο��Ѵ�.
			makeMaze();
			getFile();
			isHint = false;
		}

		// ������ ����Ű�� �����ְ�, ���������� �̵��� ������ ��� �̵���Ų��. 
		if (rightPressed && canMove(playerX + 2, playerY)) {
			playerX += 2;
		}

		// ���� ����Ű�� �����ְ�, �������� �̵��� ������ ��� �̵���Ų��.
		if (leftPressed && canMove(playerX - 2, playerY)) {
			playerX -= 2;
		}

		// ���� ����Ű�� �����ְ�, �������� �̵��� ������ ��� �̵���Ų��.
		if (upPressed && canMove(playerX, playerY - 2)) {
			playerY -= 2;
		}

		// �Ʒ��� ����Ű�� �����ְ�, �Ʒ������� �̵��� ������ ��� �̵���Ų��.
		if (downPressed && canMove(playerX, playerY + 2)) {
			playerY += 2;
		}

		// ������ ������ ��� ��� ����Ǵ� �κ��̴�.
		// ������ �� frame���� 1 ���������ְ�, �ð��� 1�ʸ��� 1�� �پ��� �����ߴ�.
		if (isOpen) {
			score += 1;
			timeChecker += 1;

			// timeChecker�� ���� FPS�� ���ٴ� ���� 1�ʰ� �귶�ٴ� ���� �ǹ��Ѵ�.
			if (timeChecker == FPS) {
				timeLeft -= 1; // �����ð��� 1 ���ҽ�Ų��.
				timeChecker = 0; // timeCheck�� 0���� �����Ѵ�.
			}
		}

		// �����̽��ٸ� ������ �� ����Ǵ� �κ��̴�.
		// �����̽��ٸ� ������ hintTimer�� 2*FPS�� �����ȴ�. �̸� �� �����Ӹ��� 1 ���ҽ�Ű�� 2���Ŀ� 0�� �ȴ�.
		if (isHint) {
			hintTimer -= 1;

			if (hintTimer == 0) {
				cout << "hint finished!" << endl;
				isHint = false;
				ofBackground(200);
				
			}
		}

		// �ð��� ����Ǿ��ٸ� ���� ����
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

	// ����ȭ���� �׷��ش�.
	case 's':
		char gameNameStr[256];
		sprintf(gameNameStr, "ENTER TO START");
		myFont.drawString(gameNameStr, ofGetWidth() / 2 - 100, ofGetHeight()/2 - 100);

		drawScoreList();


		break;

	// ����ȭ���� �׷��ش�.
	case 'g':
		drawMaze();// �̷θ� �׷��ش�.
		drawScoreTime(); // ������ �ð��� �׷��ش�.
		if (isHint) drawHint(); // �����̽��ٰ� ���ȴٸ� �����θ� 2�ʵ��� ȭ�鿡 �׷��ش�.
		drawPlayer(); // �÷��̾ �׷��ش�.
		drawEndPoint(); // �������� �׷��ش�.
		drawHintComment(); // ��Ʈ ������ �׷��ش�.

		break;
	}


} // end Draw

// �̷θ� �׷��ִ� �Լ�
void ofApp::drawMaze() {

	
	// HEIGHT��ŭ �����̸�
	for (int i = 0; i < HEIGHT; i++) {

		// WIDTH��ŭ �����̸�
		for (int j = 0; j < WIDTH; j++) {

			// ���� ���κ��� ������
			if (maze[i][j] == '-') {
				ofDrawLine(10 * (j - 1) - 2, 10 * i, 10 * (j + 1) + 2, 10 * i); // ���� �������� �翷���� �׸��� �׷��ش�.
			}

			// ���� ���κ��� ������
			else if (maze[i][j] == '|') {
				ofDrawLine(10 * j, 10 * (i - 1) - 2, 10 * j, 10 * (i + 1) + 2); // ���� �������� ���Ʒ��� �׸��� �׷��ش�.
			}

		}
	}
}

// ������ �ð��� �׷��ִ� �Լ�
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

// �����̽��ٸ� ������ ��Ʈ��θ� �׷��ִ� �Լ�
void ofApp::drawHint() {

	ofSetColor((2 * FPS - hintTimer) * 2);
	if (isOpen)
	{
		kruskalDraw(); // �ּ� ���� Ʈ���� �׷��ش�.
	}
	else
		cout << "you must open file first" << endl;
}

// �÷��̾ �׷��ִ� �Լ�
void ofApp::drawPlayer() {
	ofSetColor(0, 0, 255);
	ofDrawCircle(playerX, playerY, 5);
}

// �������� �׷��ִ� �Լ�
void ofApp::drawEndPoint() {
	ofSetColor(255, 0, 0);

	for (unsigned int i = 0; i < endPoints.size(); i++) {

		int endY = endPoints[i].first;
		int endX = endPoints[i].second;
		ofDrawCircle(10*endX, 10*endY, 5);
	}
}

// �����̽��ٸ� ������ ��Ʈ�� �� �� �ִٴ� ��Ʈ�� ȭ�鿡 �׷��ִ� �Լ�
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

	
	// 1���� �����ϸ� ȭ�鿡 ���
	if (ranking_vector.size() >= 1) {
		sprintf(first, "first place : %d ", ranking_vector[0]);
		myFont.drawString(first, ofGetWidth() - 200, 100);
	}

	// 2���� �����ϸ� ȭ�鿡 ���
	if (ranking_vector.size() >= 2) {
		sprintf(second, "second place : %d ", ranking_vector[1]);
		myFont.drawString(second, ofGetWidth() - 200, 150);
	}

	// 3���� �����ϸ� ȭ�鿡 ���
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

	// �������� ���� ��� ���� ����
	if (key == OF_KEY_RIGHT) {
		rightPressed = true;
	}

	// ������ ���� ��� ���� ����
	if (key == OF_KEY_LEFT) {
		leftPressed = true;
	}

	// ������ ���� ��� ���� ����
	if (key == OF_KEY_UP) {
		upPressed = true;
	}
		
	// �Ʒ����� ���� ��� ���� ����
	if (key == OF_KEY_DOWN) {
		downPressed = true;
	}

	// �̷�Ż�� ��Ʈ�� ���� �۾�                    
	if (key == ' ') {
		isHint = true; // ��Ʈ�� �������� ��Ȳ���� �����ϴ� ����
		hintTimer = 2 * FPS; // ��Ʈ�� 2�ʰ� ��������.

		edgeCount = 0; // ������ ��尳�� - 1���� �ɶ� ���� ũ�罺Į �˰����� ������ ���� ���� ����
		heapCount = 0; // ���� ���� ����ִ� ������ ������ �����ϴ� �����̴�.

		// union-find �˰����� ���� �θ� ��� ���� �迭
		parent = (int*)malloc(ballCount * sizeof(int));

		if (parent == NULL) {
			cout << "front failed" << endl;
			return;
		}
		for (int i = 0; i < ballCount; i++) parent[i] = -1; // union-find algorithm�� ���� -1�� �ʱ�ȭ�Ѵ�.

		// ũ�罺Į �˰����� �����Ѵ�.
		kruskal();
	}
	

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

	if ((key == OF_KEY_RETURN) && (screenVariable == 's')) {
		init(); // ���ӽ����� ���� �ʱ�ȭ������ �������ش�.
	}

	// �������� �� ��� ���� ����
	if (key == OF_KEY_RIGHT) {
		rightPressed = false;
	}

	// ������ �� ��� ���� ����
	if (key == OF_KEY_LEFT) {
		leftPressed = false;
	}

	// ������ �� ��� ���� ����
	if (key == OF_KEY_UP) {
		upPressed = false;
	}

	// �Ʒ����� �� ��� ���� ����
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


// ���� ������ ���� �ʱ�ȭ �����̴�.
void ofApp::init() {

	N = 4; // �̷��� ���� : 4
	M = 4; // �̷��� ���� : 4
	makeMaze(); // �̷������� �����Ѵ�.
	getFile(); // ������� ���Ͽ��� �̷θ� �ҷ��´�.

	// �ƹ��� ����Ű�� �������� ���� �����̴�.
	rightPressed = false;
	leftPressed = false;
	upPressed = false;
	downPressed = false;

	// ��Ʈ�� ���ۺ��� �������� �ʴ´�.
	isHint = false;

	score = 0; // ������ 0������ �����Ѵ�.

	timeLeft = 12; // ���� �ð��� �����Ѵ�. 12�ʸ� �ش�.
	timeChecker = 0; // 1�ʸ��� ȭ��� 1�� �پ��� �������̽��� ��������� �����̴�.

	FPS = 50; // �ʴ� 50 �������� �ߵ��� �����Ѵ�.
	hintTimer = 0; // ��Ʈ�� ȭ�鿡 ��õ��ȸ� ���̵��� �ϱ� ���� �����̴�.

	screenVariable = 'g'; // ����ȭ������ ��ȯ�Ѵ�.
}

// ������� �̷����Ϸκ��� �̷θ� �ҷ��ͼ� vector �ڷᱸ���� �����Ѵ�.
bool ofApp::getFile() {

	// ������ �ҷ��´�.
	ofFile file("20181288.maz");

	if (!file.exists()) {
		cout << "Target file does not exists." << endl;
		return false;
	}
	else {
		cout << "We found the target file." << endl;
		isOpen = 1;
	}

	// ���ϰ�ü�� �����Ѵ�.
	ofBuffer buffer(file);


	// �̷δ� maze��� �̸��� vector �ڷᱸ���� ��´�.
	// ������� �ʱ�ȭ�� ���ش�.
	maze.clear();
	vector<string>().swap(maze);


	// buffer�� ����� ���� ������ ���κ��� �޾ƿ���
	for (auto line : buffer.getLines()) {
		maze.push_back(line); // string Ÿ���� �� ������ ������ maze ���Ϳ� �߰��Ѵ�.
	}

	HEIGHT = maze.size(); // �̷��� HEIGHT�� maze ������ �������̴�.
	WIDTH = maze[0].size(); // �̷��� WIDTH�� maze[0] ���ڿ��� �������̴�.

	// visited ������ �迭 �����Ҵ�
	visited = new int*[HEIGHT]; // ������ �迭�� �����Ҵ� ���ֱ� ���� new�� �̿��Ѵ�.
	for (int i = 0; i < HEIGHT; i++) visited[i] = new int[WIDTH]; // ������ visited[i]�� new�� �̿��Ͽ� �����Ҵ����ش�.


	return true; // ���� �бⰡ �����߱� ������ true�� ��ȯ�Ѵ�.
}

// �޸𸮸� �������ش�.
void ofApp::freeMemory() {

	// ������ ������ ���
	if (isexit) {
		// visited �迭�� �޸𸮸� �������ִ� �κ�
		for (int x = 0; x < HEIGHT; x++) {
			delete[] visited[x]; // �������迭�� ������ �κ��� �޸𸮸� �������ְ�
		}
		delete[] visited; // �������迭�� ������ �κ��� �޸𸮸� �������ش�.
	}
}

// DFS Ž���� �ϴ� �Լ�
bool ofApp::DFS(int startY, int startX, int endY, int endX)
{
	// visited �迭�� ��� 0���� �ʱ�ȭ�Ѵ�.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0�� ���� �湮���� �ʾҴٴ� ���� �ǹ��ϰ�, 1�� �湮�ߴٴ� ���� �ǹ��Ѵ�.
	}

	// ������θ� ������ vector�� �ʱ�ȭ�Ѵ�.
	finalPathDfs.clear();
	vector<pair<int, int>>().swap(finalPathDfs);

	PathDfs.push({ startY, startX }); // �̷��� ����� (startY, startX)���� �����Ѵ�.
	visited[startY][startX] = 1; // (startY,startX)�� �湮�߱� ������ �湮ó���� ���ش�.

	// PathDfs�� �� ������ �ƴϸ� while loop�� �����Ѵ�.
	while (!PathDfs.empty()) {

		// �� ������ �����ϸ� while loop�� �������´�.
		if (PathDfs.top().first == endY  && PathDfs.top().second == endX) break;

		int nowY = PathDfs.top().first; // ������ �ֻ�ܿ� ����� ��ġ�� Y��ǥ
		int nowX = PathDfs.top().second; // ������ �ֻ�ܿ� ����� ��ġ�� X��ǥ

		int notMoved = 1; // ������ �ֻ�ܿ� ����� ��ġ�� ������ 4���⿡ ���� �̵������ϸ� 0, �̵��������� ������ 1�� �����Ѵ�.

		// ������ �ֻ�ܿ� ����� ��ġ�� ������ 4������ ��ȸ�ϸ�
		for (int i = 0; i < 4; i++) {

			int checkY = nowY + yCheck[i]; // �̵����⿡ ���� ���翩�θ� Ȯ���ϱ� ���� ��ġ�� Y��ǥ
			int checkX = nowX + xCheck[i]; // �̵����⿡ ���� ���翩�θ� Ȯ���ϱ� ���� ��ġ�� X��ǥ

			int nextY = nowY + yMove[i]; // �̵��� ������ ��ġ�� Y��ǥ
			int nextX = nowX + xMove[i]; // �̵��� ������ ��ġ�� X��ǥ

			// ���� �̵��� ������ ��ġ�� �̷θ� ����ٸ� ���� ��ġ�� Ž���Ϸ� ����.
			if (!((0 < nextY) && (nextY < HEIGHT) && (0 < nextX) && (nextX < WIDTH))) continue;

			// �̵����⿡ ���� ����, ���� �湮���� ���� ��ġ�̸� �������� �̵��� �����ϴ�.
			if (maze[checkY][checkX] == ' ' && visited[nextY][nextX] == 0) {
				PathDfs.push({ nextY, nextX }); // ���� ��ġ�� PathDfs�� push���ش�.
				visited[nextY][nextX] = 1; // ���� ��ġ�� �湮ó�����ش�.
				notMoved = 0; // ������ �������� �̵��� �����ϹǷ� 0���� �����Ѵ�.
				break;
			}
		}

		// ������ 4���� �� ���ε� �̵��� �Ұ����ϸ� PathDfs���� pop�� �����Ѵ�.
		if (notMoved) PathDfs.pop();
	}

	// PathDfs�� ����� ��ǥ�� finalPathDfs��� ���Ϳ� �����Ѵ�. �ε����� �����ϱ� �����̴�.
	while (!PathDfs.empty()) {
		int y = PathDfs.top().first; // �ֻ�ܿ� ����� ��ġ�� y��ǥ�̴�. 
		int x = PathDfs.top().second; // �ֻ�ܿ� ����� ��ġ�� x��ǥ�̴�.

		PathDfs.pop(); // �ֻ�� ���Ҹ� pop���ش�.

		finalPathDfs.push_back({ y, x }); // �ֻ�� ��ġ�� y,x��ǥ�� pair�� finalPathDfs ���Ϳ� �ڿ� �߰����ش�.
	}

	return 1; // �Լ��� �����Ѵ�.
}

// �̷��� ���� Ż�� ��θ� �׸���.
void ofApp::dfsdraw(vector<pair<int, int>> PATH)
{
	for (unsigned int i = 0; i < PATH.size() - 1; i++) {
		int pY1 = PATH[i].first; // PATH�� ����� �� ��ġ�� y��ǥ
		int pX1 = PATH[i].second; // PATH�� ����� �� ��ġ�� x��ǥ
		int pY2 = PATH[i + 1].first; //  PATH�� ����� �ٸ� ��ġ�� y��ǥ
		int pX2 = PATH[i + 1].second; // PATH�� ����� �ٸ� ��ġ�� x��ǥ

		ofDrawLine(10 * pX1, 10 * pY1, 10 * pX2, 10 * pY2); // ����� �� ��ġ�� �� ������ �Ͽ� ������ �׸���.
	}


}


// �̷θ� �����Ѵ�.
void ofApp::makeMaze() {

	srand((unsigned int)time(NULL)); // �������� ���Ῡ�θ� �����ϱ� ���� �õ�ѹ��� ��ȭ�����ִ� �κ�

	N++; // �̷��� ���� ����� 1 ������Ų��.
	M++; // �̷��� ���� ����� 1 ��ī��Ų��.
	playerX = 10; // �÷��̾��� x��ǥ
	playerY = 10; // �÷��̾��� y��ǥ


	// ������ ��ǥ�� ��� vector�� �ʱ�ȭ�Ѵ�.
	endPoints.clear();
	vector<pair<int, int>>().swap(endPoints);

	int count = 0; // ������ �������� ������ �����Ѵ�.

	// ballcount��ŭ �������� �����Ǿ��� �� loop�� �������´�.
	// �������� ���� �ߺ����� ������, ������� �������� �ʴ´�.
	while (count < ballCount) {
		int endY = rand() % N + 1;
		int endX = rand() % M + 1;

		// ���� �������� ������ �������� ��ǥ�� �߰��� �� ���� �Ǵ��ϴ� �����̴�.
		bool dontAdd = false;

		// ��� �������� ���� �ߺ����� ������, ������� �������� �ʴ´�.
		for (unsigned int i = 0; i < endPoints.size(); i++) {
			if ((endPoints[i].first == endY && endPoints[i].second == endX) || (endY == 1 && endX == 1)) {
				dontAdd = true;
				break;
			}
		}

		// �������� �ߺ��ǰų�, ������� ������ �Ǿ��ٸ� �������� �ٽ� �����Ѵ�.
		if (dontAdd) continue;

		// ���������� �������� �����Ǿ��ٸ�, endPoints ���Ϳ� �����Ѵ�.
		endPoints.push_back({ 2 * endY - 1 , 2 * endX - 1 });

		// �������� �߰��Ǿ����Ƿ� count�� 1 ������Ų��.
		count++;


	}
	roomNum = 1; // �� ��ȣ�� 1���� �����Ѵ�.
	types = 1; // ������ ������ �����Ѵ�.

	fp = fopen("./data/20181288.maz", "w"); // 20181288.maz ������ �����ϰ�, �̹� �ִٸ� ���Ӱ� �ٽ� ����.

	if (fp == NULL) {
		printf("File open Error"); // ������ �������� �ʴ´ٸ� �����޼����� ����Ѵ�.
		return; // ���α׷��� �����Ѵ�.
	}

	else {
		printf("������ �����Ǿ����ϴ�.\n"); // ������ ���������� �����Ǿ��ٸ� �����ߴٴ� �޼����� ����Ѵ�.
	}

	prevMAZE = (int*)calloc(N, sizeof(int*)); // �����Ҵ����� �̷� �� �迭 ����, 0���� �ʱ�ȭ.
	nowMAZE = (int*)calloc(N, sizeof(int*)); // �����Ҵ����� �̷� �� �迭 ����, 0���� �ʱ�ȭ.

	// ���� ����� ������ ��� ������ ���θ����ִ�.
	for (int i = 0; i <= 2 * N; i++) {

		// ¦�������� �𼭸��̹Ƿ� +�� ����Ѵ�.
		if (i % 2 == 0) {
			//printf("+");
			fprintf(fp, "+");
		}

		// Ȧ�������� �𼭸��� �ƴϹǷ� ��(-)�� ����Ѵ�.
		else {
			//printf("-");
			fprintf(fp, "-");
		}
	}

	// ���� ĭ�� �̷θ� ������ֱ� ���� newline�� ������ش�.
	//printf("\n");
	fprintf(fp, "\n");


	/* ���� ���� */
	for (int line = 0; line < M; line++) {

		horizontalCheck(line); // ���� Ȯ���ϰ� �ִ� �࿡ ���� ���� ������ ó�����ش�.


		TYPES = (int*)malloc(types * sizeof(int)); // ������ ������ ������ �迭
		COUNT = (int*)calloc(types, sizeof(int)); // ������ ������ ������ ������ �迭
		INDEX = (int*)malloc((N*M + 1) * sizeof(int)); // �����Ҵ����� �ε����� ������ �迭 ����.

		for (int i = 1; i <= N * M; i++) INDEX[i] = -1; // ������ �ε����� -1�� �ʱ�ȭ���ش�.

		int tIndex = 0; // TYPES�� ���� ������ �� �ʿ��� ����
		for (int i = 0; i < N; i++) {

			// ���� ������ ������� �ʾҴٸ�
			if (INDEX[nowMAZE[i]] == -1) {
				INDEX[nowMAZE[i]] = tIndex; // INDEX �迭�� ���� �����Ѵ�.
				TYPES[tIndex++] = nowMAZE[i]; // TYPES �迭�� ���� ������ ��ȣ�� �����Ѵ�.
			}

			COUNT[INDEX[nowMAZE[i]]]++; // COUNT �迭�� ���� 1 ������Ų��.
		}

		for (int i = 0; i < N; i++) {
			prevMAZE[i] = nowMAZE[i]; // ���� ������ �Ű��ְ�
			nowMAZE[i] = 0; // ���� ���� �ʱ�ȭ���ش�.
		}

		// ����ĭ�� �̷θ� ������־���ϱ� ������ newline�� ������ش�.
		//printf("\n");
		fprintf(fp, "\n");


		/* ���� ���� */
		verticalCheck(line); // ���������� ó�����ش�.

		// �����ٿ� �̷θ� ������ֱ� ���� newline�� ������ش�.
		//printf("\n");
		fprintf(fp, "\n");

	}

	// ������ �ݾ��ش�.
	fclose(fp);
}


// ��������� ó���ϴ� �Լ�
void ofApp::horizontalCheck(int line) {

	// ���չ�ȣ�� ������ �Ǿ� ���� ���� ĭ�� ���������� ����� roomNum�� �̿��� �Ź� �ٸ� ���� ��ȣ�� �Ű��ش�.
	for (int i = 0; i < N; i++) {
		if (nowMAZE[i] == 0) nowMAZE[i] = roomNum++;
	}

	types = 1; // ���� �࿡�� Ž���ϴ� ������ ������ 1�� �ʱ�ȭ���ش�.

	// ���� ��� ���캸��
	for (int i = 0; i <= N; i++) {

		// ���� ������ �׻� ���̴�. ���� ����� "|" �̴�.
		if (i == 0) {
			//printf("| ");
			fprintf(fp, "| ");
			continue; // for loop�� ó���κ����� ���ư���.
		}

		// �� ������ �׻� ���̴�. ���� ����� "|" �̴�.
		if (i == N) {
			//printf("|");
			fprintf(fp, "|");
			break; // for loop�� Ż���Ѵ�.
		}

		// ���� ������ ���̶��
		if (line == M - 1) {

			// ������ ĭ�� ���չ�ȣ�� �ٸ��ٸ� ��ΰ� ���ٴ� ���� �ǹ��ϹǷ� ������ ������ ���־�� �Ѵ�.
			if (nowMAZE[i - 1] != nowMAZE[i]) {

				int nowMin = min(nowMAZE[i - 1], nowMAZE[i]); // ������ �� ĭ�� ��ȣ �� ���� ��ȣ
				int nowMax = max(nowMAZE[i - 1], nowMAZE[i]); // ������ �� ĭ�� ��ȣ �� ū ��ȣ

				// ���� ��������ֱ�. �� ��ü�� ���� nowMax�� ���� ���չ�ȣ�� ������ �ִ� ĭ�� nowMin���� �������ش�.
				for (int same = 0; same < N; same++)
					if (nowMAZE[same] == nowMax) nowMAZE[same] = nowMin;

				// ���� ����Ǿ��� ������ ������ ������ش�.
				//printf("  ");
				fprintf(fp, "  ");
			}

			// ���� ����Ǿ����� �ʴٸ� ���� ������ش�. ���� "|"����̴�.
			else {
				//printf("| ");
				fprintf(fp, "| ");
			}
		}


		// ���� ���� Ȯ���ϴ� ���� ������ ���� �ƴ϶��
		else if (line < M - 1) {
			connected = rand() % 2; // ���� ���� ���� �������� �����Ѵ�.

			// ���� ������ �Ǿ� ���� �����鼭 ������ �����ַ��� �� �� -> �̹� ������ �Ǿ������� ��������ָ� �ȵȴ�.
			if (connected == 1 && (nowMAZE[i - 1] != nowMAZE[i])) {

				int nowMin = min(nowMAZE[i - 1], nowMAZE[i]); // ������ �� ĭ�� ��ȣ �� ���� ��ȣ
				int nowMax = max(nowMAZE[i - 1], nowMAZE[i]); // ������ �� ĭ�� ��ȣ �� ū ��ȣ

				// ���� ��������ֱ�. �� ��ü�� ���� nowMax�� ���� ���չ�ȣ�� ������ �ִ� ĭ�� nowMin���� �������ش�.
				for (int same = 0; same < N; same++)
					if (nowMAZE[same] == nowMax) nowMAZE[same] = nowMin;

				// ���� ����Ǿ� �����Ƿ� ������ ������ش�.
				//printf("  ");
				fprintf(fp, "  ");
			}

			// ���� ����Ǿ����� �ʰų�, ������ ���� �ʴ� ������ ������ �ߴٸ�
			else {
				// ���� ����Ѵ�. ���� "|" ����̴�.
				//printf("| ");
				fprintf(fp, "| ");
				types++; // ������ ������ �ϳ� ���������ش�.
			}
		}
	}
}

// ���������� ó���ϴ� �Լ�
void ofApp::verticalCheck(int line) {

	// ������ ���� Ž���ϰ� �ִٸ�
	if (line == M - 1) {
		for (int i = 0; i <= 2 * N; i++) {

			// ¦�������� �𼭸��̹Ƿ� +�� ����Ѵ�.
			if (i % 2 == 0) {
				//printf("+");
				fprintf(fp, "+");
			}

			// Ȧ�������� �𼭸��� �ƴϹǷ� ��(-)�� ����Ѵ�.
			else {
				//printf("-");
				fprintf(fp, "-");
			}
		}

		// ������ ���� ó������ �������Ƿ� �Լ��� �����Ѵ�.
		return;
	}

	// �Ʒ���� ������ ���Ῡ�θ� �����ϴ� �迭�̴�. 0���� �ʱ�ȭ�Ѵ�.
	CONNECTED = (int*)calloc(types, sizeof(int));

	for (int i = 0; i <= 2 * N; i++) {

		// ¦�������� �𼭸��̹Ƿ� +�� ����Ѵ�.
		if (i % 2 == 0) {
			//printf("+");
			fprintf(fp, "+");
		}

		// Ȧ�������� ���ϼ��� �ƴ� �� �� �ִ�.
		else {

			// ���� ������ ���� �࿡ �ϳ��ۿ� ���µ� ���� ������ �Ǿ����� �ʴٸ� ������ ������ �����־���Ѵ�.
			if (COUNT[INDEX[prevMAZE[i / 2]]] == 1 && CONNECTED[INDEX[prevMAZE[i / 2]]] == 0) {
				nowMAZE[i / 2] = prevMAZE[i / 2]; // ���� ���� ���չ�ȣ�� ���� ���� ���չ�ȣ�� �ǰ� �Ѵ�.
				CONNECTED[INDEX[prevMAZE[i / 2]]] = 1; // ���� ������ �Ʒ� ��� ����Ǿ������Ƿ� 1�� �������ش�.
				COUNT[INDEX[prevMAZE[i / 2]]]--; // ���� Ž�� ���� ���չ�ȣ�� ������ 1 �ٿ��ش�.
				//printf(" "); // ������ �Ǿ� �����Ƿ� ������ ������ش�.
				fprintf(fp, " ");
			}

			// ���� ������ ���� �࿡ �ϳ� �̻� ������ ������ ���� ���� �ְ� ������ ���� ���� �ִ�.
			else if (COUNT[INDEX[prevMAZE[i / 2]]] >= 1) {
				connected = rand() % 2;

				// ������ ���شٰ� ������ �ߴٸ�
				if (connected == 1) {
					nowMAZE[i / 2] = prevMAZE[i / 2]; // ���� ���� ���� ��ȣ�� ���� ���� ���չ�ȣ�� �ǰ� �Ѵ�.
					CONNECTED[INDEX[prevMAZE[i / 2]]] = 1; // ���� ������ �Ʒ� ��� ����Ǿ������Ƿ� 1�� �������ش�.
					//printf(" "); // ����Ǿ��� ������ ������ ������ش�.
					fprintf(fp, " ");
				}

				// ������ ���� �ʴ´ٰ� ������ �ߴٸ�
				else {
					//printf("-"); // ���� �׷��ش�. ���� "-" ����̴�.
					fprintf(fp, "-");
				}

				// ���� Ž���ϴ� ���չ�ȣ�� ������ 1 �ٿ��ش�.
				COUNT[INDEX[prevMAZE[i / 2]]]--;
			}
		}
	}
}


// ����Ű�� �̵��� �� �ִ� �� ���θ� �Ǵ��ϴ� �Լ�
bool ofApp::canMove(int x, int y) {

	int i, j;

	// HEIGHT��ŭ �����̸�
	for (i = 0; i < HEIGHT; i++) {

		// WIDTH��ŭ �����̸�
		for (j = 0; j < WIDTH; j++) {

			// ���� ���κ��� ������
			if (maze[i][j] == '-') {

				// �÷��̾ ���� ������ ������ �� ����.
				if ((x >= 10 * (j - 1) - 1) && (x <= 10 * (j + 1) + 1) && (y == 10 * i)) return false;
			}

			// ���� ���κ��� ������
			else if (maze[i][j] == '|') {
				// �÷��̾ ���� ������ ������ �� ����.
				if ((y > 10 * (i - 1) - 1) && (y < 10 * (i + 1) + 1) && (x == 10 * j)) return false;

			}
		}
	}

	// �̵������ϹǷ� true�� �������ش�.
	return true;
}



// �ּ� ���� Ʈ���� �����ϱ� ���� ũ�罺Į �˰����� ������ �Լ�
void ofApp::kruskal() {

	// �ּ� ���� Ʈ���� ������ ������ PATHS ���͸� �ʱ�ȭ�Ѵ�.
	PATHS.clear();
	vector<vector<pair<int, int>>>().swap(PATHS);

	// ���� �ּ� ���� Ʈ���� ������ ������ ���� checkPoints ���͸� �ʱ�ȭ�Ѵ�.
	checkPoints.clear();
	vector<pair<int, int>>().swap(checkPoints);

	// checkPoints�� endPoints�� ��� ���ҵ��� ��� �ִ�.
	checkPoints = endPoints;

	// ���� �÷��̾��� ��ġ ���� checkPoints ���Ϳ� ��´�.
	int nowPlayerY = playerY / 10;
	int nowPlayerX = playerX / 10;

	if ((nowPlayerY % 2) == 0) nowPlayerY++;
	if ((nowPlayerX % 2) == 0) nowPlayerX++;

	checkPoints.push_back({ nowPlayerY, nowPlayerX });

	heapCount = 0; // ���� ���ִ� ���Ҵ� 0���� �ʱ�ȭ�Ѵ�.


	// ��� üũ����Ʈ�� ���� DFS�� �����Ͽ� ������ �����ϴ� ������ cost�� ����Ѵ�.
	for (unsigned int i = 0; i < checkPoints.size(); i++) {

		for (unsigned int j =i+1; j < checkPoints.size(); j++) {

			// ���� ������ �ΰ��� ���� ���� DFS ��θ� ã�Ƽ� �� �� �ִ� cost�� ����Ѵ�.
			DFS(checkPoints[i].first, checkPoints[i].second, checkPoints[j].first, checkPoints[j].second);

			int p1 = i; // ���� ������
			int p2 = j; // ���� ����
			int weight = finalPathDfs.size(); // ���� �� �� ���̸� �����ϴ� ������ cost
			vector<pair<int, int>> path = finalPathDfs; // ���� �� �� ���̸� �����ϴ� ���� ���

			/* min heap �����ϱ�*/
			heapNode.head = p1; // ���� ������
			heapNode.tail = p2; // ���� ����
			heapNode.weight = -weight; //���� �� �� ���̸� �����ϴ� ������ cost -> min heap�� �����ϱ� ���� ��ũ��
			heapNode.path = path; // ���� �� �� ���̸� �����ϴ� ���� ���
			insert_heap(heapNode, &heapCount); // ������� ��带 heap�� �����Ѵ�.
		}
	}

	// heap�� ������ ������ 0�� �ƴϰ�, ���� MST�� ����� edge������ num_of_vertices-1�� �ƴ� ���
	while (heapCount != 0 && edgeCount != ballCount ) {
		delete_heap(&heapCount); // ���� �ֻ�� ��带 �����. 

		int headRoot = collapsingFind(deletedNode.head); // find �Լ��� �̿��� ���� ���ۺκ��� ��Ʈ�� ���Ѵ�.
		int tailRoot = collapsingFind(deletedNode.tail); // find �Լ��� �̿��� ���� �� �κ��� ��Ʈ�� ���Ѵ�.

		// cycle�� �߻��ϴ� edge��� �����ϰ� �ǳʶڴ�.
		if (headRoot == tailRoot) continue;

		// ������ union ���ش�.
		weightedUnion(headRoot, tailRoot);
		edgeCount++; // ������ �߰��� �Ǿ����Ƿ� 1 ������Ų��.

		// ���� ������ ������ PATHS ���Ϳ� �߰����ش�.
		PATHS.push_back(deletedNode.path);

	}

}

// ũ�罺Į �˰����� ������ ������ ������ �׷��ش�.
void ofApp::kruskalDraw() {

	// �ּ� ���� Ʈ���� �����ϴ� ��� ��θ� �׷��ش�.
	for (unsigned int i = 0; i < PATHS.size(); i++) {
		dfsdraw(PATHS[i]);
	}
}



// min heap�� ���Ҹ� �����ϴ� �Լ�
void ofApp::insert_heap(element item, int *n) {

	int i;

	// HEAP_FULL error handling
	if (HEAP_FULL(*n)) {
		printf("Heap is full.\n");
		return;
	}

	i = ++(*n);

	// ��Ʈ�� �ƴϰ�, ���� cost�� �θ��� cost���� ū ��� (������ ���� ���)�� �θ�� ��ü���ش�.
	while ((i != 1) && (item.weight > heap[i / 2].weight)) {
		heap[i] = heap[i / 2];
		i /= 2;
	}
	heap[i] = item;
}

// min heap���� ���Ҹ� �����Ѵ�.
void ofApp::delete_heap(int *n) {

	int parent, child;
	element item, temp;

	if (HEAP_EMPTY(*n)) {
		printf("Heap is empty.\n");
		return;
	}

	// �ֻ�� ��带 �̾ƿ´�.
	item = heap[1];

	temp = heap[(*n)--];
	parent = 1;
	child = 2;

	// �� �ڷᱸ���� �����ϸ� ���� �ֻ�� ��忡 ��ġ�� ��带 �Ʒ��� ����������.
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

// ��ξ������� ����� find �Լ�
int ofApp::collapsingFind(int i) {
	int root, trail, lead;

	// ���� ��Ʈ�� ã�´�.
	for (root = i; parent[root] >= 0; root = parent[root]);

	// ���� ���õ� ������ ��Ʈ���� �ö󰡸� ��� parent���� ��Ʈ�� ��ü���ش�.
	for (trail = i; trail != root; trail = lead) {
		lead = parent[trail];
		parent[trail] = root;
	}

	// ��Ʈ�� ��ȯ�Ѵ�.
	return root;
}


// �������� Ʈ�� ������ �����ִ� union�Լ�
// find ������ �ξ� ������ ������ �� �ֵ��� ���ش�.
void ofApp::weightedUnion(int i, int j) {
	int temp = parent[i] + parent[j];
	
	// ������ ������ ū ������ ��Ʈ�� ������ ������ ���� ������ ��Ʈ�� �θ� ��尡 �ǵ��� �����.
	if (parent[i] > parent[j]) {
		parent[i] = j;
		parent[j] = temp;
	}

	else {
		parent[j] = i;
		parent[i] = temp;
	}
}


// ���� ������ ranking_vector�� ������������ �����Ѵ�.
void ofApp::registerScore() {
	
	// ������ ���ҵ��� ��ȸ�ϸ� �����ؾ��ϴ� ��ġ�� ã���� insert�Լ��� �̿��Ͽ� ������ �߰��� ���Ҹ� �������־���.
	for (unsigned int i = 0; i < ranking_vector.size(); i++) {
		if (ranking_vector[i] < score) {
			ranking_vector.insert(ranking_vector.begin() + i, score);
			return;
		}
	}
	// ���� ������ ���� �ʾҴٸ� ���Ұ� �ϳ��� �����ٴ� ���� �ǹ��ϹǷ� ���Ҹ� �������ش�.
	ranking_vector.push_back(score);
	return;
}

// ranking_vector�� ����Ǿ� �ִ� �������� scores.txt�� �������ش�.
void ofApp::Vector2File() {

	fp = fopen("scores.txt", "wt"); // ���������͸� �̿��Ͽ� scores.txt������ ���� ���� ����.

	// ������ ���µ� �����ߴٸ� ���� �޼����� ������ְ� �Լ��� �����Ѵ�.
	if (fp == NULL) {
		cout << "file open error" << endl;
		return;
	}

	// ranking_vector�� ���ؼ� ��� ���ҵ��� score.txt�� ���ش�.
	for (unsigned int i = 0; i < ranking_vector.size(); i++) {
		fprintf(fp, "%d ", ranking_vector[i]);
	}

	// ������ �ݾ��ش�.
	fclose(fp);
	return;
}

//  scores.txt�� ����Ǿ� �ִ� �������� ranking_vector�� �������ش�.
void ofApp::File2Vector() {

	// scores.txt�� �б� ���� ����.
	fp = fopen("scores.txt", "rt");

	// ������ ���µ� �����ߴٸ� ���� �޼����� ������ְ� �Լ��� �����Ѵ�.
	if (fp == NULL) {
		cout << "file open error" << endl;
		return;
	}

	int fileItem; // score.txt ������ ������ ������ �����̴�.

	// ������ ������ ������ ranking_vector�� ������ �������ش�.
	while (EOF != fscanf(fp, "%d", &fileItem)) {
		ranking_vector.push_back(fileItem);
	}

	if (ranking_vector.size() >= 1)
		highScore = ranking_vector[0]; // �ְ� ������ �����Ѵ�.

	// ������ �ݾ��ش�.
	fclose(fp);
	return;
}