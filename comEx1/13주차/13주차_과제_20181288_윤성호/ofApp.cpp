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

	ofSetWindowTitle("Maze Example"); // Set the app name on the title bar
	ofSetFrameRate(15);
	ofBackground(255, 255, 255);
	// Get the window size for image loading
	windowWidth = ofGetWidth();
	windowHeight = ofGetHeight();
	isdfs = false; // dfs함수를 실행여부를 판단하는 변수. 실행되지 않았으면 false, 실행되었으면 true.
	isbfs = false; // bfs함수를 실행여부를 판단하는 변수. 실행되지 않았으면 false, 실행되었으면 true.
	isOpen = 0;  // 파일이 열렸는지 여부를 판단하는 변수
	isexit = false; // 파일이 닫혔는지 여부를 판단하는 변수.
	// Centre on the screen
	ofSetWindowPosition((ofGetScreenWidth()-windowWidth)/2, (ofGetScreenHeight()-windowHeight)/2);

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

	// Open an maze file
	menu->AddPopupItem(hPopup, "Open", false, false); // Not checked and not auto-checked
	
	// Final File popup menu item is "Exit" - add a separator before it
	menu->AddPopupSeparator(hPopup);
	menu->AddPopupItem(hPopup, "Exit", false, false);

	//
	// View popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "View");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "Show DFS",false,false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "Show BFS", false, false); // Not checked (default)
	bFullscreen = false; // not fullscreen yet
	menu->AddPopupItem(hPopup, "Full screen", false, false); // Not checked and not auto-check

	//
	// Help popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "Help");
	menu->AddPopupItem(hPopup, "About", false, false); // No auto check


	//
	// ToolBar popup menu
	//
	hPopup = menu->AddPopupMenu(hMenu, "ToolBar");

	bShowInfo = true;  // screen info display on
	menu->AddPopupItem(hPopup, "DFS", false, false); // Checked
	bTopmost = false; // app is topmost
	menu->AddPopupItem(hPopup, "BFS", false, false); // Not checked (default)

	// Set the menu to the window
	menu->SetWindowMenu();


} // end Setup


//
// Menu function
//
// This function is called by ofxWinMenu when an item is selected.
// The the title and state can be checked for required action.
// 
void ofApp::appMenuFunction(string title, bool bChecked) {

	ofFileDialogResult result;
	string filePath;
	size_t pos;

	//
	// File menu
	//
	if(title == "Open") {
		freeMemory();
		isdfs = false;
		isbfs = false;
		isOpen = 0;
		ofClear(ofColor::white);
		readFile();
	}
	if(title == "Exit") {
		isexit = true;
		freeMemory();
		ofExit(); // Quit the application
	}

	//
	// Window menu
	//
	if(title == "Show DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		//doTopmost(bChecked);
		if (isOpen)
		{
			freeMemory();
			DFS();
			bShowInfo = bChecked;
		}
		else 
			cout << "you must open file first" << endl;
		
	}

	if(title == "Show BFS") {
		//doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			freeMemory();
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if(title == "Full screen") {
		bFullscreen = !bFullscreen; // Not auto-checked and also used in the keyPressed function
		doFullScreen(bFullscreen); // But als take action immediately
	}

	//
	// Help menu
	//
	if(title == "About") {
		ofSystemAlertDialog("ofxWinMenu\nbasic example\n\nhttp://spout.zeal.co");
	}


	//
	// ToolBar menu
	//
	if (title == "DFS") {
		//bShowInfo = bChecked;  // Flag is used elsewhere in Draw()
		doTopmost(bChecked);
		if (isOpen)
		{
			freeMemory();
			DFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;

	}

	if (title == "BFS") {
		doTopmost(bChecked); // Use the checked value directly
		if (isOpen)
		{
			freeMemory();
			BFS();
			bShowInfo = bChecked;
		}
		else
			cout << "you must open file first" << endl;
	}

} // end appMenuFunction


//--------------------------------------------------------------
void ofApp::update() {

}


//--------------------------------------------------------------
void ofApp::draw() {

	char str[256];
	//ofBackground(0, 0, 0, 0);
	ofSetColor(0,0,255);
	ofSetLineWidth(5);
	int i, j;
	
	// TO DO : DRAW MAZE; 
	// 저장된 자료구조를 이용해 미로를 그린다.
	// add code here

	// HEIGHT만큼 움직이며
	for (i = 0; i < HEIGHT; i++) {

		// WIDTH만큼 움직이며
		for (j = 0; j < WIDTH; j++) {

			// 만약 가로벽을 만나면
			if (maze[i][j] == '-') {
				ofDrawLine(10*(j-1), 10*i, 10*(j + 1), 10*i); // 벽을 기준으로 양옆으로 그림을 그려준다.
			}

			// 만약 세로벽을 만나면
			else if (maze[i][j] == '|') {
				ofDrawLine(10*j, 10*(i - 1), 10*j, 10*(i + 1)); // 벽을 기준으로 위아래로 그림을 그려준다.
			}

		}
	}

	// DFS함수가 실행되었다면
	if (isdfs)
	{

		ofSetLineWidth(5); // 그리는 선의 굵기는 5로 설정한다.

		// 만약 파일이 열려있다면
		if (isOpen)
			dfsdraw(); // dfs과정에서 그려야하는 것들을 모두 그려준다.

		// 파일이 열려있지 않다면
		else
			cout << "You must open file first" << endl; // 파일을 열어주어야 한다는 에러 메세지를 출력한다.
	}
	
	// BFS함수가 실행되었다면
	if (isbfs)
	{

		ofSetLineWidth(5);// 그리는 선의 굵기는 5로 설정한다.

		// 만약 파일이 열려있다면
		if (isOpen)
			bfsdraw(); // bfs과정에서 그려야하는 것들을 모두 그려준다.

		// 파일이 열려있지 않다면
		else
			cout << "You must open file first" << endl; // 파일을 열어주어야 한다는 에러 메세지를 출력한다.
	}

	// 메뉴에 체크가 되어있다면 
	if(bShowInfo) {
		// Show keyboard duplicates of menu functions
		sprintf(str, " comsil project");
		myFont.drawString(str, 15, ofGetHeight()-20);
	}

} // end Draw


void ofApp::doFullScreen(bool bFull)
{
	// Enter full screen
	if(bFull) {
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
		ofSetWindowPosition((ofGetScreenWidth()-ofGetWidth())/2, (ofGetScreenHeight()-ofGetHeight())/2);
		// Show the cursor again
		ofShowCursor();
		// Restore topmost state
		if(bTopmost) doTopmost(true);
	}

} // end doFullScreen


void ofApp::doTopmost(bool bTop)
{
	if(bTop) {
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
		if(GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE); 
	}
} // end doTopmost


//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	
	// Escape key exit has been disabled but it can be checked here
	if(key == VK_ESCAPE) {
		// Disable fullscreen set, otherwise quit the application as usual
		if(bFullscreen) {
			bFullscreen = false;
			doFullScreen(false);
		}
		else {
			ofExit();
		}
	}

	// Remove or show screen info
	if(key == ' ') {
		bShowInfo = !bShowInfo;
		// Update the menu check mark because the item state has been changed here
		menu->SetPopupItem("Show DFS", bShowInfo);
	}

	if(key == 'f') {
		bFullscreen = !bFullscreen;	
		doFullScreen(bFullscreen);
		// Do not check this menu item
		// If there is no menu when you call the SetPopupItem function it will crash
	}

} // end keyPressed

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
bool ofApp::readFile()
{
	ofFileDialogResult openFileResult = ofSystemLoadDialog("Select .maz file");

	string filePath;
	size_t pos;
	// Check whether the user opened a file
	if (openFileResult.bSuccess) {
		ofLogVerbose("User selected a file");

		//We have a file, check it and process it
		string fileName = openFileResult.getName();
		//string fileName = "maze0.maz";
		cout << "file name is " << fileName << endl;
		filePath = openFileResult.getPath();
		printf("Open\n");
		pos = filePath.find_last_of(".");
		if (pos != string::npos && pos != 0 && filePath.substr(pos + 1) == "maz") {

			ofFile file(fileName);

			if (!file.exists()) {
				cout << "Target file does not exists." << endl;
				return false;
			}
			else {
				cout << "We found the target file." << endl;
				isOpen = 1;
			}


			ofBuffer buffer(file);
			
			
			// TO DO
			// .maz 파일을 input으로 받아서 적절히 자료구조에 넣는다.

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

			/*
			for (size_t i = 0; i < maze.size(); i++) {
				cout << maze[i] << "\n";
			}
			cout << endl;
			*/

			// BFS에서 최적경로를 찾기위해서 필요한 부모노드 저장하는 3차원 배열
			Parent = new int**[HEIGHT];// 3차원 배열을 동적할당 해주기 위해 new를 이용한다.
			for (int i = 0; i < HEIGHT; i++) {
				Parent[i] = new int*[WIDTH]; // Parent[i]를 동적할당 해주기 위해 new를 이용한다.
				for (int j = 0; j < WIDTH; j++) Parent[i][j] = new int[2]; // Parent[i][j]를 동적할당 해주기 위해 new를 이용한다.
			}

			return true; // 파일 읽기가 성공했기 때문에 true를 반환한다.
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

///////  -----> vector, stack, queue 메모리 해제해주기!
void ofApp::freeMemory() {

	//TO DO
	// malloc한 memory를 free해주는 함수
	
	/*
	if (isOpen == 0) {
		maze.clear()
		vector<string>().swap(maze);
	}
	*/

	if (isexit) {
		// visited 배열의 메모리를 해제해주는 부분
		for (int x = 0; x < HEIGHT; x++) {
			delete[] visited[x]; // 이차원배열이 구현된 부분의 메모리를 해제해주고
		}
		delete[] visited; // 일차원배열이 구현된 부분의 메모리를 해제해준다.

		// Parent 배열의 메모리를 해제해주는 부분
		for (int x = 0; x < HEIGHT; x++) {

			for (int y = 0; y < WIDTH; y++) {
				delete[] Parent[x][y]; // 삼차원배열이 구현된 부분의 메모리를 해제해주고
			}
		}
		for (int x = 0; x < HEIGHT; x++) {
			delete[] Parent[x]; // 이차원배열이 구현된 부분의 메모리를 해제해주고
		}
		delete[] Parent; // 일차원배열이 구현된 부분의 메모리를 해제해준다.
	}

	
	// 현재 dfs 탐색을 한 상황에서 메모리 해제를 해주어야 한다.
	if (isdfs) {
		// PathDfs의 원소를 모두 삭제
		while (!PathDfs.empty()) PathDfs.pop();

		// DFS로 찾은 최단 경로를 저장한 finalPathDfs의 메모리를 해제해주고 초기화해준다.
		finalPathDfs.clear(); // 원소들을 모두 지워준다.
		vector<pair<int, int>>().swap(finalPathDfs); // 새롭게 정의해준다.

		// DFS 탐색경로를 저장한 배열의 메모리를 해제해준다.
		checkDFS.clear(); // 원소들을 모두 지워준다.
		vector<vector<int>>().swap(checkDFS); // 새롭게 정의해준다.


		// dfs 탐색이 종료되었으므로 false로 설정해준다.
		isdfs = false;
	}

	// 현재 bfs 탐색을 한 상황에서 메모리 해제를 해주어야 한다.
	if (isbfs) {
		// PathBfs의 원소를 모두 삭제
		while (!PathBfs.empty()) PathBfs.pop();

		// BFS 탐색경로를 저장한 배열의 메모리를 해제해준다.
		checkBFS.clear(); // 원소들을 모두 지워준다.
		vector<vector<int>>().swap(checkBFS); // 새롭게 정의해준다.

		// bfs 탐색이 종료되었으므로 false로 설정해준다.
		isbfs = false;
	}


}

bool ofApp::DFS()//DFS탐색을 하는 함수
{
	//TO DO
	//DFS탐색을 하는 함수 ( 3주차)

	// visited 배열을 모두 0으로 초기화한다.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0은 아직 방문하지 않았다는 것을 의미하고, 1은 방문했다는 것을 의미한다.
	}
	
	PathDfs.push({ 1,1 }); // 미로의 출발은 (1,1)에서 시작한다.
	visited[1][1] = 1; // (1,1)은 방문했기 때문에 방문처리를 해준다.

	// PathDfs가 빈 스택이 아니면 while loop를 실행한다.
	while (!PathDfs.empty()) {


		// 끝 지점에 도달하면 while loop를 빠져나온다.
		if (PathDfs.top().first == HEIGHT - 2 && PathDfs.top().second == WIDTH - 2) break;

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
				vector <int> newPath = { nowX, nowY, nextX, nextY }; // 현재 탐색하는 경로를 newPath에 저장한다. 
				checkDFS.push_back(newPath); // checkDFS 벡터에 추가해준다. 
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

	isdfs = true; // DFS함수가 실행되었기때문에 true로 설정해준다.

	return 1; // 함수를 종료한다.
}

//DFS를 수행한 결과를 그린다.
void ofApp::dfsdraw()
{

	ofSetColor(200);// 적당히 희미한 회색으로 DFS과정 중 탐색한 모든경로를 그린다.

	// DFS과정 중 탐색한 모든경로를 그린다.
	for (unsigned int i = 0; i < checkDFS.size(); i++) {
		int x1 = checkDFS[i][0]; // checkDFS에 저장된 한 위치의 x좌표
		int y1 = checkDFS[i][1]; // checkDFS에 저장된 한 위치의 y좌표
		int x2 = checkDFS[i][2]; // checkDFS에 저장된 다른 위치의 x좌표
		int y2 = checkDFS[i][3]; // checkDFS에 저장된 다른 위치의 y좌표

		ofDrawLine(10*x1, 10*y1, 10*x2, 10*y2); // 저장된 두 위치를 양 끝으로 하여 선분을 그린다.
	}

	ofSetColor(0); // 실제 미로 탈출 경로는 검은색으로 그린다.

	// 실제 미로 탈출 경로를 그린다.
	for (unsigned int i = 0; i < finalPathDfs.size() - 1; i++) {
		int pY1 = finalPathDfs[i].first; //  finalPathDfs에 저장된 한 위치의 x좌표
		int pX1 = finalPathDfs[i].second; //  finalPathDfs에 저장된 한 위치의 y좌표
		int pY2 = finalPathDfs[i+1].first; //  finalPathDfs에 저장된 다른 위치의 x좌표
		int pX2 = finalPathDfs[i+1].second; //  finalPathDfs에 저장된 다른 위치의 y좌표

		ofDrawLine(10*pX1, 10*pY1, 10*pX2, 10*pY2); // 저장된 두 위치를 양 끝으로 하여 선분을 그린다.
	}


}

//BFS탐색을 하는 함수
bool ofApp::BFS()
{

	// visited 배열을 모두 0으로 초기화한다.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0은 아직 방문하지 않았다는 것을 의미하고, 1은 방문했다는 것을 의미한다.
	}


	PathBfs.push({ 1,1 }); // 미로의 출발은 (1,1)에서 시작한다.
	visited[1][1] = 1; // (1,1)은 방문했기 때문에 방문처리를 해준다.

	// PathBfs가 빈 큐가 아니면 while loop를 실행한다.
	while (!PathBfs.empty()) {


		int nowY = PathBfs.front().first; // 큐의 가장 앞에 저장된 위치의 Y좌표
		int nowX = PathBfs.front().second; // 큐의 가장 앞에 저장된 위치의 X좌표

		// 끝 지점에 도달하면 종료한다.
		if (nowY == HEIGHT - 2 && nowX == WIDTH - 2) break;

		// 가장 앞에 저장된 원소를 pop 해준다.
		PathBfs.pop();

		// 큐의 가장 앞에 저장된 위치의 인접한 4방향을 순회하며
		for (int i = 0; i < 4; i++) {


			int checkY = nowY + yCheck[i]; // 이동방향에 벽의 존재여부를 확인하기 위한 위치의 Y좌표
			int checkX = nowX + xCheck[i]; // 이동방향에 벽의 존재여부를 확인하기 위한 위치의 X좌표

		
			int nextY = nowY + yMove[i]; // 이동할 예정인 위치의 Y좌표
			int nextX = nowX + xMove[i]; // 이동할 예정인 위치의 X좌표


			// 이동할 예쩡인 위치가 미로를 벗어난다면 다음 이동위치의 후보로 넘어간다.
			if (!((0 < nextY) && (nextY < HEIGHT) && (0 < nextX) && (nextX < WIDTH))) continue;

			// 빈칸이면 다음으로 이동 할 수도 있다.
			if (maze[checkY][checkX] == ' ' && visited[nextY][nextX] == 0) {
				PathBfs.push({ nextY, nextX }); // PathBfs 배열에 다음 위치를 push_back 해준다.
				visited[nextY][nextX] = 1; // 다음 위치를 방문처리 해준다.
				vector <int> newPath = { nowX, nowY, nextX, nextY }; // 탐색한 경로를 int형 vector로 만든다.
				checkBFS.push_back(newPath); // 탐색한 경로정보를 checkBFS에 push_back 해준다.
				Parent[nextY][nextX][0] = nowY; // 다음 위치의 Y값의 부모는 현재 위치의 Y값으로 설정해준다.
				Parent[nextY][nextX][1] = nowX; // 다음 위치의 X값의 부모는 현재 위치의 X값으로 설정해준다.

			}
		}
	}

	// bfs 탐색을 진행했기 때문에 true로 설정해준다.
	isbfs = true;

	return 1;
}

//BFS를 수행한 결과를 그린다.
void ofApp::bfsdraw()
{

	// 탐색경로를 그릴 색깔을 진한 회색이다.
	ofSetColor(200);

	// bfs 알고리즘으로 탐색한 경로를 저장한 checkBFS를 순회하며 모두 그려준다.
	for (unsigned int i = 0; i < checkBFS.size(); i++) {
		int x1 = checkBFS[i][0]; // checkBFS에 저장된 한 위치의 x좌표
		int y1 = checkBFS[i][1]; // checkBFS에 저장된 한 위치의 y좌표
		int x2 = checkBFS[i][2]; // checkBFS에 저장된 다른 위치의 x좌표
		int y2 = checkBFS[i][3]; // checkBFS에 저장된 다른 위치의 y좌표

		ofDrawLine(10 * x1, 10 * y1, 10 * x2, 10 * y2); // 저장된 두 위치를 양 끝으로 하여 선분을 그린다.
	}
	

	// 실제 최단 경로는 검은색으로 그린다.
	ofSetColor(0);

	// 최종지점은 미로의 오른쪽 하단이다.
	int pY1 = HEIGHT - 2; // 가장 아래지점을 나타낸다.
	int pX1 = WIDTH - 2; // 가장 오른쪽지점을 나타낸다.

	// (1,1)에 다다르기 전까지 while loop를 수행한다.
	while (pX1 != 1 || pY1 != 1){

		int pY2 = Parent[pY1][pX1][0]; // 부모의 y값
		int pX2 = Parent[pY1][pX1][1]; // 부모의 x값

		ofDrawLine(10 * pX1, 10 * pY1, 10 * pX2, 10 * pY2); // 저장된 두 위치를 양 끝으로 하여 선분을 그린다.

		// 부모의 부모를 찾아가기 위해 현재 좌표를 부모의 좌표로 바꿔준다.
		pY1 = pY2; // 현재 y좌표를 부모의 y좌표로 바꿔준다.
		pX1 = pX2; // 현재 x좌표를 부모의 x좌표로 바꿔준다.
	}
}
