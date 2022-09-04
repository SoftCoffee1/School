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
	isdfs = false; // dfs�Լ��� ���࿩�θ� �Ǵ��ϴ� ����. ������� �ʾ����� false, ����Ǿ����� true.
	isbfs = false; // bfs�Լ��� ���࿩�θ� �Ǵ��ϴ� ����. ������� �ʾ����� false, ����Ǿ����� true.
	isOpen = 0;  // ������ ���ȴ��� ���θ� �Ǵ��ϴ� ����
	isexit = false; // ������ �������� ���θ� �Ǵ��ϴ� ����.
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
	// ����� �ڷᱸ���� �̿��� �̷θ� �׸���.
	// add code here

	// HEIGHT��ŭ �����̸�
	for (i = 0; i < HEIGHT; i++) {

		// WIDTH��ŭ �����̸�
		for (j = 0; j < WIDTH; j++) {

			// ���� ���κ��� ������
			if (maze[i][j] == '-') {
				ofDrawLine(10*(j-1), 10*i, 10*(j + 1), 10*i); // ���� �������� �翷���� �׸��� �׷��ش�.
			}

			// ���� ���κ��� ������
			else if (maze[i][j] == '|') {
				ofDrawLine(10*j, 10*(i - 1), 10*j, 10*(i + 1)); // ���� �������� ���Ʒ��� �׸��� �׷��ش�.
			}

		}
	}

	// DFS�Լ��� ����Ǿ��ٸ�
	if (isdfs)
	{

		ofSetLineWidth(5); // �׸��� ���� ����� 5�� �����Ѵ�.

		// ���� ������ �����ִٸ�
		if (isOpen)
			dfsdraw(); // dfs�������� �׷����ϴ� �͵��� ��� �׷��ش�.

		// ������ �������� �ʴٸ�
		else
			cout << "You must open file first" << endl; // ������ �����־�� �Ѵٴ� ���� �޼����� ����Ѵ�.
	}
	
	// BFS�Լ��� ����Ǿ��ٸ�
	if (isbfs)
	{

		ofSetLineWidth(5);// �׸��� ���� ����� 5�� �����Ѵ�.

		// ���� ������ �����ִٸ�
		if (isOpen)
			bfsdraw(); // bfs�������� �׷����ϴ� �͵��� ��� �׷��ش�.

		// ������ �������� �ʴٸ�
		else
			cout << "You must open file first" << endl; // ������ �����־�� �Ѵٴ� ���� �޼����� ����Ѵ�.
	}

	// �޴��� üũ�� �Ǿ��ִٸ� 
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
			// .maz ������ input���� �޾Ƽ� ������ �ڷᱸ���� �ִ´�.

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

			/*
			for (size_t i = 0; i < maze.size(); i++) {
				cout << maze[i] << "\n";
			}
			cout << endl;
			*/

			// BFS���� ������θ� ã�����ؼ� �ʿ��� �θ��� �����ϴ� 3���� �迭
			Parent = new int**[HEIGHT];// 3���� �迭�� �����Ҵ� ���ֱ� ���� new�� �̿��Ѵ�.
			for (int i = 0; i < HEIGHT; i++) {
				Parent[i] = new int*[WIDTH]; // Parent[i]�� �����Ҵ� ���ֱ� ���� new�� �̿��Ѵ�.
				for (int j = 0; j < WIDTH; j++) Parent[i][j] = new int[2]; // Parent[i][j]�� �����Ҵ� ���ֱ� ���� new�� �̿��Ѵ�.
			}

			return true; // ���� �бⰡ �����߱� ������ true�� ��ȯ�Ѵ�.
		}
		else {
			printf("  Needs a '.maz' extension\n");
			return false;
		}
	}
}

///////  -----> vector, stack, queue �޸� �������ֱ�!
void ofApp::freeMemory() {

	//TO DO
	// malloc�� memory�� free���ִ� �Լ�
	
	/*
	if (isOpen == 0) {
		maze.clear()
		vector<string>().swap(maze);
	}
	*/

	if (isexit) {
		// visited �迭�� �޸𸮸� �������ִ� �κ�
		for (int x = 0; x < HEIGHT; x++) {
			delete[] visited[x]; // �������迭�� ������ �κ��� �޸𸮸� �������ְ�
		}
		delete[] visited; // �������迭�� ������ �κ��� �޸𸮸� �������ش�.

		// Parent �迭�� �޸𸮸� �������ִ� �κ�
		for (int x = 0; x < HEIGHT; x++) {

			for (int y = 0; y < WIDTH; y++) {
				delete[] Parent[x][y]; // �������迭�� ������ �κ��� �޸𸮸� �������ְ�
			}
		}
		for (int x = 0; x < HEIGHT; x++) {
			delete[] Parent[x]; // �������迭�� ������ �κ��� �޸𸮸� �������ְ�
		}
		delete[] Parent; // �������迭�� ������ �κ��� �޸𸮸� �������ش�.
	}

	
	// ���� dfs Ž���� �� ��Ȳ���� �޸� ������ ���־�� �Ѵ�.
	if (isdfs) {
		// PathDfs�� ���Ҹ� ��� ����
		while (!PathDfs.empty()) PathDfs.pop();

		// DFS�� ã�� �ִ� ��θ� ������ finalPathDfs�� �޸𸮸� �������ְ� �ʱ�ȭ���ش�.
		finalPathDfs.clear(); // ���ҵ��� ��� �����ش�.
		vector<pair<int, int>>().swap(finalPathDfs); // ���Ӱ� �������ش�.

		// DFS Ž����θ� ������ �迭�� �޸𸮸� �������ش�.
		checkDFS.clear(); // ���ҵ��� ��� �����ش�.
		vector<vector<int>>().swap(checkDFS); // ���Ӱ� �������ش�.


		// dfs Ž���� ����Ǿ����Ƿ� false�� �������ش�.
		isdfs = false;
	}

	// ���� bfs Ž���� �� ��Ȳ���� �޸� ������ ���־�� �Ѵ�.
	if (isbfs) {
		// PathBfs�� ���Ҹ� ��� ����
		while (!PathBfs.empty()) PathBfs.pop();

		// BFS Ž����θ� ������ �迭�� �޸𸮸� �������ش�.
		checkBFS.clear(); // ���ҵ��� ��� �����ش�.
		vector<vector<int>>().swap(checkBFS); // ���Ӱ� �������ش�.

		// bfs Ž���� ����Ǿ����Ƿ� false�� �������ش�.
		isbfs = false;
	}


}

bool ofApp::DFS()//DFSŽ���� �ϴ� �Լ�
{
	//TO DO
	//DFSŽ���� �ϴ� �Լ� ( 3����)

	// visited �迭�� ��� 0���� �ʱ�ȭ�Ѵ�.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0�� ���� �湮���� �ʾҴٴ� ���� �ǹ��ϰ�, 1�� �湮�ߴٴ� ���� �ǹ��Ѵ�.
	}
	
	PathDfs.push({ 1,1 }); // �̷��� ����� (1,1)���� �����Ѵ�.
	visited[1][1] = 1; // (1,1)�� �湮�߱� ������ �湮ó���� ���ش�.

	// PathDfs�� �� ������ �ƴϸ� while loop�� �����Ѵ�.
	while (!PathDfs.empty()) {


		// �� ������ �����ϸ� while loop�� �������´�.
		if (PathDfs.top().first == HEIGHT - 2 && PathDfs.top().second == WIDTH - 2) break;

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
				vector <int> newPath = { nowX, nowY, nextX, nextY }; // ���� Ž���ϴ� ��θ� newPath�� �����Ѵ�. 
				checkDFS.push_back(newPath); // checkDFS ���Ϳ� �߰����ش�. 
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

	isdfs = true; // DFS�Լ��� ����Ǿ��⶧���� true�� �������ش�.

	return 1; // �Լ��� �����Ѵ�.
}

//DFS�� ������ ����� �׸���.
void ofApp::dfsdraw()
{

	ofSetColor(200);// ������ ����� ȸ������ DFS���� �� Ž���� ����θ� �׸���.

	// DFS���� �� Ž���� ����θ� �׸���.
	for (unsigned int i = 0; i < checkDFS.size(); i++) {
		int x1 = checkDFS[i][0]; // checkDFS�� ����� �� ��ġ�� x��ǥ
		int y1 = checkDFS[i][1]; // checkDFS�� ����� �� ��ġ�� y��ǥ
		int x2 = checkDFS[i][2]; // checkDFS�� ����� �ٸ� ��ġ�� x��ǥ
		int y2 = checkDFS[i][3]; // checkDFS�� ����� �ٸ� ��ġ�� y��ǥ

		ofDrawLine(10*x1, 10*y1, 10*x2, 10*y2); // ����� �� ��ġ�� �� ������ �Ͽ� ������ �׸���.
	}

	ofSetColor(0); // ���� �̷� Ż�� ��δ� ���������� �׸���.

	// ���� �̷� Ż�� ��θ� �׸���.
	for (unsigned int i = 0; i < finalPathDfs.size() - 1; i++) {
		int pY1 = finalPathDfs[i].first; //  finalPathDfs�� ����� �� ��ġ�� x��ǥ
		int pX1 = finalPathDfs[i].second; //  finalPathDfs�� ����� �� ��ġ�� y��ǥ
		int pY2 = finalPathDfs[i+1].first; //  finalPathDfs�� ����� �ٸ� ��ġ�� x��ǥ
		int pX2 = finalPathDfs[i+1].second; //  finalPathDfs�� ����� �ٸ� ��ġ�� y��ǥ

		ofDrawLine(10*pX1, 10*pY1, 10*pX2, 10*pY2); // ����� �� ��ġ�� �� ������ �Ͽ� ������ �׸���.
	}


}

//BFSŽ���� �ϴ� �Լ�
bool ofApp::BFS()
{

	// visited �迭�� ��� 0���� �ʱ�ȭ�Ѵ�.
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++)
			visited[i][j] = 0; // 0�� ���� �湮���� �ʾҴٴ� ���� �ǹ��ϰ�, 1�� �湮�ߴٴ� ���� �ǹ��Ѵ�.
	}


	PathBfs.push({ 1,1 }); // �̷��� ����� (1,1)���� �����Ѵ�.
	visited[1][1] = 1; // (1,1)�� �湮�߱� ������ �湮ó���� ���ش�.

	// PathBfs�� �� ť�� �ƴϸ� while loop�� �����Ѵ�.
	while (!PathBfs.empty()) {


		int nowY = PathBfs.front().first; // ť�� ���� �տ� ����� ��ġ�� Y��ǥ
		int nowX = PathBfs.front().second; // ť�� ���� �տ� ����� ��ġ�� X��ǥ

		// �� ������ �����ϸ� �����Ѵ�.
		if (nowY == HEIGHT - 2 && nowX == WIDTH - 2) break;

		// ���� �տ� ����� ���Ҹ� pop ���ش�.
		PathBfs.pop();

		// ť�� ���� �տ� ����� ��ġ�� ������ 4������ ��ȸ�ϸ�
		for (int i = 0; i < 4; i++) {


			int checkY = nowY + yCheck[i]; // �̵����⿡ ���� ���翩�θ� Ȯ���ϱ� ���� ��ġ�� Y��ǥ
			int checkX = nowX + xCheck[i]; // �̵����⿡ ���� ���翩�θ� Ȯ���ϱ� ���� ��ġ�� X��ǥ

		
			int nextY = nowY + yMove[i]; // �̵��� ������ ��ġ�� Y��ǥ
			int nextX = nowX + xMove[i]; // �̵��� ������ ��ġ�� X��ǥ


			// �̵��� ������ ��ġ�� �̷θ� ����ٸ� ���� �̵���ġ�� �ĺ��� �Ѿ��.
			if (!((0 < nextY) && (nextY < HEIGHT) && (0 < nextX) && (nextX < WIDTH))) continue;

			// ��ĭ�̸� �������� �̵� �� ���� �ִ�.
			if (maze[checkY][checkX] == ' ' && visited[nextY][nextX] == 0) {
				PathBfs.push({ nextY, nextX }); // PathBfs �迭�� ���� ��ġ�� push_back ���ش�.
				visited[nextY][nextX] = 1; // ���� ��ġ�� �湮ó�� ���ش�.
				vector <int> newPath = { nowX, nowY, nextX, nextY }; // Ž���� ��θ� int�� vector�� �����.
				checkBFS.push_back(newPath); // Ž���� ��������� checkBFS�� push_back ���ش�.
				Parent[nextY][nextX][0] = nowY; // ���� ��ġ�� Y���� �θ�� ���� ��ġ�� Y������ �������ش�.
				Parent[nextY][nextX][1] = nowX; // ���� ��ġ�� X���� �θ�� ���� ��ġ�� X������ �������ش�.

			}
		}
	}

	// bfs Ž���� �����߱� ������ true�� �������ش�.
	isbfs = true;

	return 1;
}

//BFS�� ������ ����� �׸���.
void ofApp::bfsdraw()
{

	// Ž����θ� �׸� ������ ���� ȸ���̴�.
	ofSetColor(200);

	// bfs �˰������� Ž���� ��θ� ������ checkBFS�� ��ȸ�ϸ� ��� �׷��ش�.
	for (unsigned int i = 0; i < checkBFS.size(); i++) {
		int x1 = checkBFS[i][0]; // checkBFS�� ����� �� ��ġ�� x��ǥ
		int y1 = checkBFS[i][1]; // checkBFS�� ����� �� ��ġ�� y��ǥ
		int x2 = checkBFS[i][2]; // checkBFS�� ����� �ٸ� ��ġ�� x��ǥ
		int y2 = checkBFS[i][3]; // checkBFS�� ����� �ٸ� ��ġ�� y��ǥ

		ofDrawLine(10 * x1, 10 * y1, 10 * x2, 10 * y2); // ����� �� ��ġ�� �� ������ �Ͽ� ������ �׸���.
	}
	

	// ���� �ִ� ��δ� ���������� �׸���.
	ofSetColor(0);

	// ���������� �̷��� ������ �ϴ��̴�.
	int pY1 = HEIGHT - 2; // ���� �Ʒ������� ��Ÿ����.
	int pX1 = WIDTH - 2; // ���� ������������ ��Ÿ����.

	// (1,1)�� �ٴٸ��� ������ while loop�� �����Ѵ�.
	while (pX1 != 1 || pY1 != 1){

		int pY2 = Parent[pY1][pX1][0]; // �θ��� y��
		int pX2 = Parent[pY1][pX1][1]; // �θ��� x��

		ofDrawLine(10 * pX1, 10 * pY1, 10 * pX2, 10 * pY2); // ����� �� ��ġ�� �� ������ �Ͽ� ������ �׸���.

		// �θ��� �θ� ã�ư��� ���� ���� ��ǥ�� �θ��� ��ǥ�� �ٲ��ش�.
		pY1 = pY2; // ���� y��ǥ�� �θ��� y��ǥ�� �ٲ��ش�.
		pX1 = pX2; // ���� x��ǥ�� �θ��� x��ǥ�� �ٲ��ش�.
	}
}
