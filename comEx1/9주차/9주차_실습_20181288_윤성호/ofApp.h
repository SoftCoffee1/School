#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
        /* WaterFall-related member variables Regions */
        
        // flag variables
        int draw_flag;
        int load_flag;
        
        // Line segment and dot related variables
        int num_of_line = -1;
        int num_of_dot = 0;
        int flag = 0;
        int **LINES;
        int **DOTS;
        float dot_diameter;
        int dotFlag = 0;
        
        /* WaterFall-related member functions */
        
        void processOpenFileSelection(ofFileDialogResult openFileResult);
        void initializeWaterLines(); // 2nd week portion.
};
