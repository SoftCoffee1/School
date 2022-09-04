#include "tetris.h"

static struct sigaction act, oact;

int main(){
	int exit=0;

	initscr();
	noecho();
	keypad(stdscr, TRUE);	

	srand((unsigned int)time(NULL));

	while(!exit){
		clear();
		switch(menu()){
		case MENU_PLAY: play(); break;
		case MENU_EXIT: exit=1; break;
		default: break;
		}
	}

	endwin();
	system("clear");
	return 0;
}

void InitTetris(){
	int i,j;

	for(j=0;j<HEIGHT;j++)
		for(i=0;i<WIDTH;i++)
			field[j][i]=0;

	nextBlock[0]=rand()%7;
	nextBlock[1]=rand()%7;
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

	DrawOutline();
	DrawField();
	DrawBlock(blockY,blockX,nextBlock[0],blockRotate,' ');
	DrawNextBlock(nextBlock);
	PrintScore(score);
}

void DrawOutline(){	
	int i,j;
	/* 블럭이 떨어지는 공간의 태두리를 그린다.*/
	DrawBox(0,0,HEIGHT,WIDTH);

	/* next block을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(9,WIDTH+10);
	printw("SCORE");
	DrawBox(10,WIDTH+10,1,8);
}

int GetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
	case KEY_UP:
		break;
	case KEY_DOWN:
		break;
	case KEY_LEFT:
		break;
	case KEY_RIGHT:
		break;
	case ' ':	/* space key*/
		/*fall block*/
		break;
	case 'q':
	case 'Q':
		command = QUIT;
		break;
	default:
		command = NOTHING;
		break;
	}
	return command;
}

int ProcessCommand(int command){
	int ret=1;
	int drawFlag=0;
	switch(command){
	case QUIT:
		ret = QUIT;
		break;
	case KEY_UP:
		if((drawFlag = CheckToMove(field,nextBlock[0],(blockRotate+1)%4,blockY,blockX)))
			blockRotate=(blockRotate+1)%4;
		break;
	case KEY_DOWN:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY+1,blockX)))
			blockY++;
		break;
	case KEY_RIGHT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX+1)))
			blockX++;
		break;
	case KEY_LEFT:
		if((drawFlag = CheckToMove(field,nextBlock[0],blockRotate,blockY,blockX-1)))
			blockX--;
		break;
	default:
		break;
	}
	if(drawFlag) DrawChange(field,command,nextBlock[0],blockRotate,blockY,blockX);
	return ret;	
}

void DrawField(){
	int i,j;
	for(j=0;j<HEIGHT;j++){
		move(j+1,1);
		for(i=0;i<WIDTH;i++){
			if(field[j][i]==1){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(".");
		}
	}
}


void PrintScore(int score){
	move(11,WIDTH+11);
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
	for( i = 0; i < 4; i++ ){
		move(4+i,WIDTH+13);
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
	}
}

void DrawBlock(int y, int x, int blockID,int blockRotate,char tile){
	int i,j;
	for(i=0;i<4;i++)
		for(j=0;j<4;j++){
			if(block[blockID][blockRotate][i][j]==1 && i+y>=0){
				move(i+y+1,j+x+1);
				attron(A_REVERSE);
				printw("%c",tile);
				attroff(A_REVERSE);
			}
		}

	move(HEIGHT,WIDTH+10);
}

void DrawBox(int y,int x, int height, int width){
	int i,j;
	move(y,x);
	addch(ACS_ULCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_URCORNER);
	for(j=0;j<height;j++){
		move(y+j+1,x);
		addch(ACS_VLINE);
		move(y+j+1,x+width+1);
		addch(ACS_VLINE);
	}
	move(y+j+1,x);
	addch(ACS_LLCORNER);
	for(i=0;i<width;i++)
		addch(ACS_HLINE);
	addch(ACS_LRCORNER);
}

void play(){
	int command;
	clear();
	act.sa_handler = BlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
                        timed_out = 1;
		}

		command = GetCommand();
		if(ProcessCommand(command)==QUIT){
			alarm(0);
			DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
			move(HEIGHT/2,WIDTH/2-4);
			printw("Good-bye!!");
			refresh();
			getch();

			return;
		}
	}while(!gameOver);

	alarm(0);
	getch();
	DrawBox(HEIGHT/2-1,WIDTH/2-5,1,10);
	move(HEIGHT/2,WIDTH/2-4);
	printw("GameOver!!");
	refresh();
	getch();
	newRank(score);
}

char menu(){
	printw("1. play\n");
	printw("2. rank\n");
	printw("3. recommended play\n");
	printw("4. exit\n");
	return wgetch(stdscr);
}

/////////////////////////첫주차 실습에서 구현해야 할 함수/////////////////////////

int CheckToMove(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

        /////////////////////////////////////////////////////////////////
        // 현재 블록을 움직일 수 없는 경우                             //
        // 1. 예상된 현재 블록이 필드 바깥으로 나갈 경우               //
        // 2. 예상 현재 블록이 쌓여 있는 블록과 겹치는 경우            //
        /////////////////////////////////////////////////////////////////
        
        // 각각의 블록에 대하여
        for (int i = 0 ; i < 4; i++){
                for (int j = 0; j < 4 ; j++){
                        
                        // 4X4 block prototype에서 블록이 있는 지점에 대해서
                        if ( block[currentBlock][blockRotate][i][j] == 1 ){
                                
                                // 예상된 현재 블록이 필드 바깥으로 나갈 경우 움직일 수 없으므로 0을 리턴한다.
                                if ((0 > i + blockY) || (HEIGHT <= i + blockY) || (0 > j + blockX) || (WIDTH <= j + blockX))
                                        return 0;
                            
                                // 예상 현재 블록이 쌓여 있는 블록과 겹치는 경우 움직일 수 없으므로 0을 리턴한다.
                                if (f[i+blockY][j+blockX] == 1)
                                        return 0;
                        }
                }
        }
        
        // 예상된 현재 블록이 모두 필드 안에 있고 쌓여 있는 블록과 겹치지 않은 경우 움직일 수 있으므로 1을 리턴한다.
        return 1;
}

void DrawChange(char f[HEIGHT][WIDTH],int command,int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//1. 이전 블록 정보를 찾는다. ProcessCommand의 switch문을 참조할 것
	//2. 이전 블록 정보를 지운다. DrawBlock함수 참조할 것.
	//3. 새로운 블록 정보를 그린다. 
        
        //////////////////////////////  1. 이전 블록 정보를 찾는다. ////////////////////////////

        // 이전 블록의 위치, 회전횟수를 얻기 위해 이전 command를 살펴본다.
        // preBlockRotate : 이전 블록의 회전횟수
        // preBlockX : 이전 블록의 x좌표
        // preBlockY : 이전 블록의 y좌표

        int preBlockRotate; // 이전 블록의 회전 횟수
        int preBlockX; // 이전 블록의 x좌표 
        int preBlockY; // 이전 블록의 y좌표
        switch(command){
        
        // 윗 방향키이면 블록이 반시계방향으로 90도 회전했다는 것을 의미하고,
        // 이전 방향은 시계방향으로 90도 회전 시키면 된다.
        case KEY_UP:
                preBlockRotate = (blockRotate+3) % 4;
                preBlockX = blockX;
                preBlockY = blockY;
                break;

        // 아래 방향키이면 블록이 아래로 한칸 내려왔다는 것을 의미하고,
        // 이전 위치는 y좌표가 1  줄어든 위치이다. x좌표는 변함이 없다.
        case KEY_DOWN:
                preBlockRotate = blockRotate;
                preBlockX = blockX;
                preBlockY = blockY - 1;
                break;
        
        // 오른쪽 방향키이면 블록이 오른쪽으로 한칸 이동했다는 것을 의미하고,
        // 이전 위치는 x좌표가 1 줄어든  위치이다. y좌표는 변함이 없다.        
        case KEY_RIGHT:
                preBlockRotate = blockRotate;
                preBlockX = blockX - 1;
                preBlockY = blockY;
                break;

        // 왼쪽 방향키이면 블록이 왼쪽으로 한칸 이동했다는 것을 의미하고,
        // 이전 위치는 x좌표가 1 늘어난 위치이다. x좌표는 변함이 없다.
        case KEY_LEFT:
                preBlockRotate = blockRotate;
                preBlockX = blockX + 1;
                preBlockY = blockY;
                break;
        
        // 다른것이 입력되었다면 따로 변화는 없다.
        default:
                break;
        }
         
        //////////////////////////// 2. 이전 블록 정보를 지운다. ///////////////////////////

        // DrawBlock 함수를 참조했는데, 바꾼 점은 tile parameter가 "."로 고정이 되어있다는 점이다. 공백을 출력해야 하기 때문이다.
        int i, j;
        for (i = 0 ; i < 4 ; i++){
                for (j = 0; j < 4 ; j++){
                        if (block[currentBlock][preBlockRotate][i][j] == 1 && i + preBlockY >= 0){ // 이전 블록의 모양은 공백으로 바꿔준다. 
                                move(i+preBlockY+1, j+preBlockX+1); // 커서를 이전의 블럭이 있던 위치로 옮겨준다.
                                printw("."); // 공백의 의미로 ."."을 그려준다.
                        }
                        
                }
        }
        
        //////////////////////////// 3. 새로운 블록 정보를 그린다. /////////////////////////
        DrawBlock(blockY, blockX, currentBlock, blockRotate, ' ');
}

void BlockDown(int sig){
	// user code

	//강의자료 p26-27의 플로우차트를 참고한다.
                
        // 블록이 한 칸 내려갈 수 있는 경우
        if (CheckToMove(field, nextBlock[0], blockRotate, blockY+1, blockX) == 1){
                
                // 블록을 한 칸 내리고
                blockY++;
                
                // 한 칸 아래로 내려온 블록을 콘솔에 그려준다.
                DrawChange(field, KEY_DOWN, nextBlock[0], blockRotate, blockY, blockX);
                
                // 시간을 초기화해준다.
                timed_out = 0;
                return;
        }
        
        // 블록이 한 칸 내려갈 수 없는 경우
        
        
        // 블록의 y좌표가 -1 이면 gameOver = 1 로 변경해준다.
        if (blockY == -1) gameOver = 1;
        
        // 더 이상 내려갈 수 없음이 보장되었으므로, 현재의 블럭 정보를 필드에 반영해준다.
        AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        
        // 추가점수를 반영해준다.
        score += DeleteLine(field);
        
        // 점수를 화면상에 그려준다.
        PrintScore(score);
        
        // 다음블럭을 현재블록으로 만들어주고, 다음블록은 0부터 6사이의 임의의 수를 저장해준다.
        nextBlock[0] = nextBlock[1];
        nextBlock[1] = rand() % 7;
        
        // 다음 블록을 화면에 그려준다.
        DrawNextBlock(nextBlock);
        
        // 새롭게 내려올 블럭의 정보를 초기화해준다.
        blockRotate = 0; // 회전횟수 : 0
        blockY = -1; // y값 : -1
        blockX = WIDTH/2 - 2; // x값 : 화면의 중간값
        
        // 현재 필드정보를 블럭에 그려준다.
        DrawField();
        
        // 커서를 화면 바깥으로 빼준다.
        move(HEIGHT, WIDTH+10);
        
        // 시간을 초기화해준다.
        timed_out = 0;
        
        return;
        
        
}

void AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
        
        // block이 있는 곳을 field에도 반영해준다.
        for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                        if (block[currentBlock][blockRotate][i][j] == 1)
                                f[blockY+i][blockX+j] = 1; // 1은 블럭이 존재함을 의미한다.
                }
        }
}

int DeleteLine(char f[HEIGHT][WIDTH]){
	// user code
        
	//1. 필드를 탐색하여, 꽉 찬 구간이 있는지 탐색한다.
	//2. 꽉 찬 구간이 있으면 해당 구간을 지운다. 즉, 해당 구간으로 필드값을 한칸씩 내린다.
        
        int isFull; // 현재 탐색중인 줄이 완전히 채워져 있는지의 여부를 담는 변수
        int deletedLines = 0; // 지운 line의 개수를 저장하는 변수. 0으로 초기화해준다.
        int addedScore; // 추가 될 점수를 저장하는 변수
        
        // 각각의 행을 탐색하여, 완전히 채워진 줄이 있으면 isFull 가 1이므로 그 줄을 삭제해주고, 채워지지 않은 줄은 isFull 변수가 0이므로 다음줄을 탐색하러 간다.
        for (int i =0; i < HEIGHT; i++){
                isFull = 1; // 기본적으로 isFull 변수는 1로 설정해준다. 1이면 삭제를 진행해준다.
                for (int j = 0; j < WIDTH ; j++){
                        if (f[i][j] == 0){ // 완전히 채워지지 않은 줄이면
                                isFull = 0; // 채워지지 않았다는 의미로 isFull 변수를 0으로 설정해준다.
                                break; // 루프를 탈출한다.
                        }
                }
                
                // 현재 탐색 중인 줄이 완전히 채워져 있다면 
                if (isFull){
                        
                        // 지운 line의 수를 하나 늘려준다.
                        deletedLines++;
                        
                        // k는 i부터 0까지 변화하는 변수이고, 
                        // k번째 줄에k-1번째 줄의 내용을 덮어씌워준다.
                        for (int k = i; k >= 0; k--){
                                if (k == 0){ // 가장 윗줄은 더 윗줄에서 가져올 정보가 없으므로 0(빈칸으로 정보를 초기화해준다.)
                                        for (int l = 0; l < WIDTH; l++)
                                                f[k][l] = 0; // 빈칸의 의미로 0을 넣어준다.                               
                                }
                                else{ // 가장 윗줄이 아니라면
                                        for (int l = 0 ; l < WIDTH ; l++)
                                                f[k][l] = f[k-1][l]; // 윗 행의 정보를 아랫줄에 덮어씌워준다.         
                                }
                        }
                }
                        
        }
        
        // (지워진라인)^2 * 100 를 추가점수로 부여해준다.
        addedScore = deletedLines*deletedLines*100;
        return addedScore; // 추가점수를 반환해준다.
}

///////////////////////////////////////////////////////////////////////////

void DrawShadow(int y, int x, int blockID,int blockRotate){
	// user code
}

void createRankList(){
	// user code
}

void rank(){
	// user code
}

void writeRankFile(){
	// user code
}

void newRank(int score){
	// user code
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	// user code
}

int recommend(RecNode *root){
	int max=0; // 미리 보이는 블럭의 추천 배치까지 고려했을 때 얻을 수 있는 최대 점수

	// user code

	return max;
}

void recommendedPlay(){
	// user code
}
