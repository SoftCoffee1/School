#include "tetris.h"

static struct sigaction act, oact;
int B,count;

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
                case MENU_RANK: rank(); break;
                case MENU_REC : recommendedPlay(); break;
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
        
        // VISIBLE_BLOCKS 개의 블록을 랜덤생성한다. recommend 함수에서 사용하기 위함이다.
        for (int n = 0; n <= VISIBLE_BLOCKS; n++) nextBlock[n] = rand()%7;
	
	blockRotate=0;
	blockY=-1;
	blockX=WIDTH/2-2;
	score=0;	
	gameOver=0;
	timed_out=0;

        // recRoot를 초기화해준다.
        recRoot = (RecNodePointer)malloc(sizeof(RecNode));
        recRoot->lv = 0;
        recRoot->score = 0;
        recRoot->f = field;
        recRoot->blockID = nextBlock[0]; 
       
        RecNodePointer *c = recRoot->c;
        for (int i = 0; i < CHILDREN_MAX; i++){
                c[i] = (RecNodePointer)malloc(sizeof(RecNode));
                c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*WIDTH*HEIGHT);
        }
        
        // recRoot를 가지고 추 좌표를 얻어낸다.
        recommend(recRoot);

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

	/* next block1을 보여주는 공간의 태두리를 그린다.*/
	move(2,WIDTH+10);
	printw("NEXT BLOCK");
	DrawBox(3,WIDTH+10,4,8);

    /* next block2를 보여주는 공간의 테두리를 그린다.*/
    DrawBox(9, WIDTH+10, 4, 8);

	/* score를 보여주는 공간의 태두리를 그린다.*/
	move(15,WIDTH+10);
	printw("SCORE");
	DrawBox(16,WIDTH+10,1,8);
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
	move(17,WIDTH+11); // 점수를 출력할 위치를 수정하였다.
	printw("%8d",score);
}

void DrawNextBlock(int *nextBlock){
	int i, j;
        
        /*
        int n = sizeof(nextrBlock) / sizeof(int);
	for( i = 0; i < 4; i++ ){
                
                for (int added = 4, int num = 1; num <= n; added+=6, num++){
                        move(added+i, WIDTH+13);
		        for( j = 0; j < 4; j++ ){
			        if( block[nextBlock[num]][0][i][j] == 1 ){
				        attron(A_REVERSE);
				        printw(" ");
				        attroff(A_REVERSE);
			        }
			        else printw(" ");
		        }
                }
        */
        // nextBlock[1] 그리기
        for (i = 0; i < 4; i++){
		move(4+i,WIDTH+13); // 커서를 y축방향을 4+i만큼, x축 방향으로 WIDTH+13만큼 떨어진 지점으로 옮긴다.
		for( j = 0; j < 4; j++ ){
			if( block[nextBlock[1]][0][i][j] == 1 ){
				attron(A_REVERSE);
				printw(" ");
				attroff(A_REVERSE);
			}
			else printw(" ");
		}
               
        // nextBlock[2] 그리기
                move(10+i, WIDTH+13); // 커서를 y축방향을 10+i만큼, x축 방향으로 WIDTH+13만큼 떨어진 지점으로 옮긴다.
                for (j = 0; j < 4 ; j++){
                        if ( block[nextBlock[2]][0][i][j] == 1 ){
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

        
        // 이전 위치에 해당하는 그림자를 지워준다.
        int tempY = preBlockY; // 이전 그림자의 y좌표
        int tempX = preBlockX; // 이전 그림자의 x좌표
        int tempR = preBlockRotate; // 이전 그림자의 회전횟수


		// 이전 그림자의 정보를 구한다.
        while (CheckToMove(field, currentBlock, tempR, tempY+1, tempX)) tempY += 1;

		// 이전 그림자의 정보를 얻어온 후, 그림자가 있던 자리에 '.'를 그려주며 그림자를 삭제해준다.
        for ( i=0; i < 4 ; i++){
                for (j = 0; j < 4 ; j++){
                        if (block[currentBlock][tempR][i][j] == 1 && i + tempY >= 0){
                                move(i+tempY+1, j+tempX+1);
                                printw(".");
                        }
                }
        }
        
        
        //////////////////////////// 3. 새로운 블록 정보와 그림자를 그린다. /////////////////////////
        DrawBlockWithFeatures(blockY, blockX, currentBlock, blockRotate);
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
        score += AddBlockToField(field, nextBlock[0], blockRotate, blockY, blockX);
        
        // 추가점수를 반영해준다.
        score += DeleteLine(field);
        
        // 점수를 화면상에 그려준다.
        PrintScore(score);
        
        // 다음블럭을 현재블록으로 만들어주고, 다다음블록을 다음블록으로 만들어주고, 다다음블록은 0부터 6사이의 임의의 수를 저장해준다.
        for (int n = 0; n < VISIBLE_BLOCKS; n++) nextBlock[n] = nextBlock[n+1];
        nextBlock[VISIBLE_BLOCKS] = rand()%7;
        
        
        recRoot = (RecNodePointer)malloc(sizeof(RecNode));
        recRoot->lv = 0;
        recRoot->score = 0;
        recRoot->f = field;
        recRoot->blockID = nextBlock[0]; 
       
        RecNodePointer *c = recRoot->c;
        for (int i = 0; i < CHILDREN_MAX; i++){
                c[i] = (RecNodePointer)malloc(sizeof(RecNode));
                c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*WIDTH*HEIGHT);
        } 
         
        recommend(recRoot);
       
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

int AddBlockToField(char f[HEIGHT][WIDTH],int currentBlock,int blockRotate, int blockY, int blockX){
	// user code

	//Block이 추가된 영역의 필드값을 바꾼다.
        
        int touched = 0; // 필드와 블록의 아랫부분이 맞닿은 부분의 개수를 저장하는 변수이다.
        int score; // 새로운 점수 계산방식에 의해 계산된 점수를 저장하는 변수이다.
        // block이 있는 곳을 field에도 반영해준다.
        for (int i = 0; i < 4; i++){
                for (int j = 0; j < 4; j++){
                        if (block[currentBlock][blockRotate][i][j] == 1){
                                f[blockY+i][blockX+j] = 1; // 1은 블럭이 존재함을 의미한다.
                                
                                if ( blockY+i+1 >= HEIGHT) touched += 1; // 블럭이 필드의 바닥에 닿았다면 touched 변수를 1 증가시킨다.
                                else if (f[blockY+i+1][blockX+j] == 1) touched += 1; // 필드의 바닥이 아닌데, 현재 블록의 아래부분에 블록이 존재한다면 touched 변수를 1 증가시킨다. 
                        }
                }
        }
        
		// 새로운 점수방식으로 점수를 계산한 후 리턴값으로 준다.
        score = touched * 10;
        return score;
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
	
        
        // 더 이상 내려갈 수 없을 때의 위치를 찾기 위한 반복문이다.
        while (CheckToMove(field, blockID, blockRotate, y+1, x)) y+= 1;
        
        // 더 이상 내려갈 수 없는 위치를 찾았다면 그 지점에 '/'로 그림자를 그려준다.
        DrawBlock(y, x, blockID, blockRotate, '/');              
}

void DrawBlockWithFeatures(int y, int x, int blockID, int blockRotate){
        
        DrawRecommend(recommendY, recommendX, blockID, recommendR);

        // 블록의 새로운 위치를 그려준다.
        DrawBlock(y, x, blockID, blockRotate, ' ');

        // 블록의 새로운 그림자를 그려준다.
        DrawShadow(y, x, blockID, blockRotate);
}


void createRankList(){
	// 목적: Input파일인 "rank.txt"에서 랭킹 정보를 읽어들임, 읽어들인 정보로 랭킹 목록 생성
	// 1. "rank.txt"열기
	// 2. 파일에서 랭킹정보 읽어오기
	// 3. LinkedList로 저장
	// 4. 파일 닫기
	FILE *fp;
	int i, j;

	//1. 파일 열기
	fp = fopen("rank.txt", "r");
        
        // 파일이 존재하지 않으면 score_number = 0으로 설정해준다.
        if (fp == NULL){
                score_number = 0;
                return;
        }

	// 2. 정보읽어오기
	/* int fscanf(FILE* stream, const char* format, ...);
	stream:데이터를 읽어올 스트림의 FILE 객체를 가리키는 파일포인터
	format: 형식지정자 등등
	변수의 주소: 포인터
	return: 성공할 경우, fscanf 함수는 읽어들인 데이터의 수를 리턴, 실패하면 EOF리턴 */
	// EOF(End Of File): 실제로 이 값은 -1을 나타냄, EOF가 나타날때까지 입력받아오는 if문
        
        // 현재 linkedList의 길이를 저장하는 변수가 rank.txt 파일의 가장 윗줄에 적혀있다.
        // score_number을 입력한다.
        fscanf(fp, "%d", &score_number);
        
        // 추가입력여부를 판단하기 위한 변수
        sn = score_number;

        // 이름과 점수정보를 받아온다.
        char tempName[NAMELEN]; // 받아올 이름을 임시로 저장하는 변수
        int tempScore; // 받아올 점수를 임시로 저장하는 변수
        nodePointer node, prevNode = NULL; // linkedList 자료구조로 저장하기 위해 필요한 노드포인터들

	while (fscanf(fp, "%s %d", tempName, &tempScore) != EOF) {
                node = (nodePointer)malloc(sizeof(struct _node)); // 노드생성
                strcpy(node->name, tempName); // 노드에 이름을 저장
                node->score = tempScore; // 노드에 점수를 저장
                node->link = NULL; // 기본적으로 노드의 link는 NULL로 초기화

                // 만약 첫번째 노드를 만든 경우라면 head가 node가 되어야한다.
                if (prevNode == NULL) head = node;
                
                // 그렇지 않다면 이전 노드의 링크를 현재 노드로 걸어주어야한다.
                else prevNode->link = node;
                
                // 현재노드를 이전노드로 만들어준다.
                prevNode = node;
	}


	// 4. 파일닫기
	fclose(fp);
}

void rank(){
        // rankList를 linkedList로 만든다.
        createRankList();

        //목적: rank 메뉴를 출력하고 점수 순으로 X부터~Y까지 출력함
	//1. 문자열 초기화
	int X = 1, Y=score_number, ch, i, j;

	clear();

	//2. printw()로 3개의 메뉴출력
        printw("1. list ranks from X to Y\n");
        printw("2. list ranks by a specific name\n");
        printw("3. delete a specific rank\n");


	//3. wgetch()를 사용하여 변수 ch에 입력받은 메뉴번호 저장
        noecho();
        ch = wgetch(stdscr);

	//4. 각 메뉴에 따라 입력받을 값을 변수에 저장
	//4-1. 메뉴1: X, Y를 입력받고 적절한 input인지 확인 후(X<=Y), X와 Y사이의 rank 출력
	if (ch == '1') {
                
                // X를 입력하라는 문구를 출력한다.
                printw("X: ");
                
                // 입력하는 문자가 화면에 출력되게 한다.
                echo();

                // X를 입력받는다.
                scanw("%d", &X);
                        
                // Y를 입력하라는 문구를 출력한다.
                printw("Y: ");
                
                // 입력하는 문자가 화면에 출력되게 한다.
                echo();
                
                // Y를 입력받는다.
                scanw("%d", &Y);

                // 출력형식을 정해준다.
                printw("\n");
                printw("          name          |     score     \n");
                printw("----------------------------------------\n");
                
                
                // X>Y이면 출력이 불가능하므로 에러메세지를 출력한다.
                if (X > Y || score_number == 0 || X > score_number){  
                        printw("\nsearch failure: no rank in the list");
                        getch();
                        return;
                }
                // X와 Y 사이의 이름과 점수를 출력한다.
                int i;
                nodePointer curr = head;
                
                // X까지 탐색노드를 이동시켜준다.
                for (i = 1; i < X; i++) curr = curr->link;
                
                // Y까지 탐색노드를 이동시키면서 이름과 점수를 출력한다.
                while (curr){
                        if (i <= Y){
                                printw(" %-23s| %-20d\n", curr->name, curr->score); // 이름과 점수를 형식에 맞게 출력한다.
                                curr = curr->link; // 탐색노드를 한칸 뒤로 이동시켜준다.
                                i++; // i를 1증가시켜준다.
                        }
                        
                        // 범위를 벗어나면 반복문을 빠져나온다.
                        else break;
               
	        }
        }

	//4-2. 메뉴2: 문자열을 받아 저장된 이름과 비교하고 이름에 해당하는 리스트를 출력
	else if ( ch == '2') {
		char str[NAMELEN+1];
		int check = 0;
                
                // 이름을 입력받는 부분
                printw("input the name: ");
                echo();
                scanw("%s", str);
                
                // 탐색을 위한 노드를 선언한다.
                nodePointer curr = head;
                
                // 이름과 점수를 출력하기 전에 가장 위에 적을 기본적인 틀이다.
                printw("\n");
                printw("          name          |     score     \n");
                printw("----------------------------------------\n");
                
                // 일치하는 이름이 있는지 여부를 체크하기 위한 변수이다.
                int notFound = 1;

                // linked list를 첫번째 노드부터 마지막노드까지 탐색하는 반복문
                while (curr){
                        
                        // 일치하는 이름이 발견되었다면
                        if (strcmp(curr->name, str) == 0){
                                notFound = 0; // 발견되었으므로 0으로 설정해주고
                                printw(" %-23s| %-20d\n", curr->name, curr->score); // 이름과 점수를 출력해준다.
                        }
                        
                        // 다음 노드로 넘어가준다.
                        curr = curr->link;
	        }
                
                // 만약 일치하는 이름이 발견되지 않았다면, 에러메세지를 출력해준다.
                if (notFound) printw("\nsearch failure: no information in the list");
        
        }
        
	//4-3. 메뉴3: rank번호를 입력받아 리스트에서 삭제
	else if ( ch == '3') {
		int num; 
                
                // 랭크 입력을 받는다.
                printw("input the rank: ");
                echo();
                scanw("%d", &num);
                // 유요하지 않은 랭크 입력이면 에러메세지를 출력한다.
                if (num > score_number || num < 1){
                        printw("\nsearch failure: the rank not in the list");
                        getch();
                        return;
                }
                
                
                nodePointer prev = NULL; // 이전 노드를 NULL pointer로 설정한다.
                nodePointer curr = head; // 현재 노드를 head node로 설정한다.
                nodePointer temp; // 만약 삭제해야할 노드가 첫번째 노드라면 temp에 head의 메모리를 저장해주고 메모리 해제를 위해 사용한다.

                // 삭제해야하는 랭킹의 노드까지 이동한다.
                for (int i = 1; i < num; i++){
                        prev = curr;
                        curr = curr->link;
                }

                // 삭제해야하는 노드가 첫번째 노드
                if (curr == head) {
                        temp = head; // temp에 head를 저장해주고,
                        head = head->link; // head를 다음 링크로 옮겨준다.
                        free(temp); // temp에 저장되어 있는 메모리를 해제해준다.
                }
                
                // 삭제해야하는 노드가 첫번째 노드가 아닌경우
                else{
                        prev->link = curr->link; // 이전 노드의 링크를 현재 노드의 링크가 되게 한다.
                        free(curr); // 현재 노드의 메모리를 해제해준다.
                }
                
                // 삭제 성공 메세지를 출력해준다.
                printw("\nresult: the rank deleted\n");
                
                // score_number를 1 감소시켜준다.
                score_number--;

                // 현재 linked list의 내용을 rank.txt에 업데이트해준다.
                writeRankFile(); 
        }
	getch();
}
void writeRankFile(){
	// 목적: 추가된 랭킹 정보가 있으면 새로운 정보를 "rank.txt"에 쓰고 없으면 종료

	//1. "rank.txt" 연다
	FILE *fp = fopen("rank.txt", "w+");
        
        // 추가된 랭킹 정보가 없으면 종료한다.
        if (sn == score_number) return;

	//2. 랭킹 정보들의 수를 "rank.txt"에 기록
        fprintf(fp, "%d\n", score_number);
	//3. 탐색할 노드가 더 있는지 체크하고 있으면 다음 노드로 이동, 없으면 종료
	nodePointer curr = head;
        
        // 노드를 탐색하며 파일에 입력한다.
        while (curr){
                fprintf(fp, "%s %d\n", curr->name, curr->score);
                curr = curr->link;
        }
        
        fclose(fp);
}

void newRank(int score){

        createRankList();

	// 목적: GameOver시 호출되어 사용자 이름을 입력받고 score와 함께 리스트의 적절한 위치에 저장
	char str[NAMELEN+1];
	int i, j;
	clear();
	//1. 사용자 이름을 입력받음
        printw("your name: ");
        echo();
        scanw("%s", str);
        
        // 새로운 노드를 생성한다.
        nodePointer node;
        node = (nodePointer)malloc(sizeof(struct _node));
        strcpy(node->name, str); // 이름을 생성한다.
        node->score = score; // 점수를 생성한다.
        node->link = NULL; // 링크는 NULL로 초기화한다.
        
        nodePointer prev = NULL; // 이전 노드를 NULL로 초기화한다.
        nodePointer curr = head; // curr 노드를 head로 초기화한다.
        
                
	//2. 새로운 노드를 생성해 이름과 점수를 저장, score_number가
        int done = 0; // insert여부를 체크하는 변수
        
        // 노드를 탐색하며 
	while (curr){
                // 만약 현재 점수가 더 크다면 삽입한다.
                if (curr->score <= score){
                        done = 1; // 삽입되었으므로 done변수를 1로 설정해준다.

                        // 만약 첫번째노드라면
                        if (prev == NULL) {
                                node->link = head; // 새로운 노드의 링크를 head로 걸어주고
                                head = node; // head노드를 node로 설정해준다.
                        }
                        
                        // head node가 아니라면
                        else{
                                node->link = prev->link; // 새로운 노드의 링크를 이전 노드의 링크로 초기화해준다.
                                prev->link = node; // 이전 링크의 링크를 현재 노드로 걸어준다.
                        }
                        
                        break;
                }

                prev = curr; // 이전 노드를 현재노드로 설정해주고
                curr = curr->link; // 다음 노드를 탐색한다.
        }
        // 첫번째ㄴ 노드이면 head를 노드로 설정해준다.
        if (head == NULL) head = node;
                
        // 마지막자리에 삽입해주어야 하면 이전 링크를 노드로 설정해준다.
        else if (done == 0) prev->link = node;

        score_number += 1; // score_number 1증가시킨다.
	writeRankFile(); // 함수를 실행한다.
}

void DrawRecommend(int y, int x, int blockID,int blockRotate){
	DrawBlock(y, x, blockID, blockRotate, 'R');
}
/*
void makeTree(RecNodePointer *root){
        (*root) = (RecNodePointer)malloc(sizeof(RecNode));
        (*root)->lv = 0;
        (*root)->score = 0;
        (*root)->f = field;
        (*root)->blockID = nextBlock[0]; 
       
        RecNodePointer *c = (*root)->c;
        for (int i = 0; i < CHILDREN_MAX; i++){
                c[i] = (RecNodePointer)malloc(sizeof(RecNode));
                c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*WIDTH*HEIGHT);
        }
}
*/

int recommend(RecNode *root){
        
        int max = 0; // 최댓값을 저장하는 변수
        int tempVal = 0; // 최댓값을 매번 갱신해주는 변수
        int lv = root->lv; // 레벨을 저장하는 변수
        int score = root->score; // 점수를 저장하는 변수
        RecNodePointer *c = root->c; // 자식 노드들을 생성

        int blockID = root->blockID; // 블럭 ID를 저장하는 변수
        
        /*
        if ((CheckToMove(root->f, blockID, blockRotate, blockY, blockX) == 0) && (lv == 0)){
                printw("X : %d\nY : %d\nR : %d\n\n", blockX, blockY, blockRotate);
                recommendY = -1;
                printw("detected!!");
                return 0;
        } */      

        int count = 0; // 자식노드의 인덱스를 저장하는 변수
        
        // 모든 회전 경우의 수에 대해서
        for (int tempR = 0; tempR < 4 ; tempR++){
                
                // x값이 -1인 순간부터 가장 끝 부분까지 고려한다.
                for (int tempX = -1 ;tempX <= WIDTH; tempX++){
                        // y값은 -1로 초기화해준다.
                        int tempY = -1;
                        // 전혀 움직일 수 없다면 자식노드를 만들필요가 없으므로 이를 위한 변수를 생성해준다.
                        int cannotMove = 1;
                        
                        // 현재 X값에서 y값을 최대한 내린다.
                        while (CheckToMove(root->f, blockID, tempR, tempY+1, tempX)){
                                tempY += 1;
                                cannotMove = 0;
                        }
                        
                        // 만약 y값이 한번도 내려지지 못했다면 자식노드를 생성하지 않는다.
                        if (cannotMove) continue;
                        
                        // 자식노드의 레벨은 부모 노드의 레벨보다 1 크다.
                        c[count]->lv = lv+1;
                        
                        
                        // 필드를 복사해온다.
                        for(int i=0; i < HEIGHT; i++)
                                for (int j = 0; j < WIDTH; j++)
                                       c[count]->f[i][j] = root->f[i][j];
                                             
                        // 추가된 점수를 addedScore 변수에 저장한다.  
                        int addedScore = AddBlockToField(c[count]->f, blockID, tempR, tempY, tempX);
                        addedScore += DeleteLine(c[count]->f);

                        // 자식노드의 점수는 부모노드의 점수 추가된 점수를 더해준다.
                        c[count]->score = score + addedScore;
                        
                        // 자식노드의 ID는 다음에 등장할 블록의 ID이다.
                        c[count]->blockID = nextBlock[c[count]->lv];

                        // 자식노드의 자식노드에 동적할당을 해준다.
                        RecNodePointer *nodeC = c[count]->c;
                        for (int i=0; i < CHILDREN_MAX; i++){
                                nodeC[i] = (RecNodePointer)malloc(sizeof(RecNode));
                                nodeC[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*WIDTH*HEIGHT);
                        }

                        // 가장 깊숙히 들어왔다면 더이상 진행하지않고 점수를 tempVal에 저장해준다.
                        if (c[count]->lv == VISIBLE_BLOCKS) {
                                tempVal = c[count]->score;
                        }
                        
                        // 그렇지 않다면 다음 레벨로 들어간다.
                        else {
                                tempVal = recommend(c[count]);
                        
                        }
                
                        // 자식노드의 인덱스를 1 증가시켜준다.
                        count++;

                        // 최댓값이 갱신되는 상황이면
                        if (max < tempVal){
                                
                                // 최댓값을 갱신해주고
                                max = tempVal;

                                // 가장 레벨이 낮다면 추천좌표를 배정해준다.
                                if (lv == 0){
                                        recommendX = tempX;
                                        recommendY = tempY;
                                        recommendR = tempR;
                                }
                        }
                        
                        // 추천된좌표들의 점수가 같을때는 가장 아래에 배치한다.
                        else if (max == tempVal && recommendY < tempY && lv == 0){
                                recommendX = tempX;
                                recommendY = tempY;
                                recommendR = tempR;
                        }                 
                }
        }
        
        // 최댓값을 리턴한다.
        return max;
}

void recommendBlockDown(int sig){
	// user code
        if (blockY == -1){

                recRoot = (RecNodePointer)malloc(sizeof(RecNode));
                recRoot->lv = 0;
                recRoot->score = 0;
                recRoot->f = field;
                recRoot->blockID = nextBlock[0]; 
       
                RecNodePointer *c = recRoot->c;
                for (int i = 0; i < CHILDREN_MAX; i++){
                        c[i] = (RecNodePointer)malloc(sizeof(RecNode));
                        c[i]->f = (char(*)[WIDTH])malloc(sizeof(char)*WIDTH*HEIGHT);
                }
        
         
                recommend(recRoot);

                if (recommendY == -1){
                         gameOver = 1;
                         return;
                }

                DrawRecommend(recommendY, recommendX, nextBlock[0], recommendR);

                // 블록의 새로운 위치를 그려준다.
                DrawBlock(blockY++, blockX, nextBlock[0], blockRotate, ' ');
                
                printw("node count : %d", COUNT);
                COUNT = 0;
                timed_out = 0;
                return;
        }
        

        
        // 블록의 y좌표가 -1 이면 gameOver = 1 로 변경해준다.
        //if (recommendY == -1) gameOver = 1;
        
        // 더 이상 내려갈 수 없음이 보장되었으므로, 현재의 블럭 정보를 필드에 반영해준다.
        score += AddBlockToField(field, nextBlock[0], recommendR, recommendY, recommendX);
        
        // 추가점수를 반영해준다.
        score += DeleteLine(field);
        
        // 점수를 화면상에 그려준다.
        PrintScore(score);
        
        // 다음블럭을 현재블록으로 만들어주고, 다다음블록을 다음블록으로 만들어주고, 다다음블록은 0부터 6사이의 임의의 수를 저장해준다.
        for (int n = 0; n < VISIBLE_BLOCKS; n++) nextBlock[n] = nextBlock[n+1];
        nextBlock[VISIBLE_BLOCKS] = rand()%7;
        //nextBlock[0] = nextBlock[1];
        //nextBlock[1] = nextBlock[2];
        //nextBlock[2] = rand() % 7;
       
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

int recGetCommand(){
	int command;
	command = wgetch(stdscr);
	switch(command){
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

void recommendedPlay(){
	// user code
	int command;
	clear();
	act.sa_handler = recommendBlockDown;
	sigaction(SIGALRM,&act,&oact);
	InitTetris();
	do{
		if(timed_out==0){
			alarm(1);
                        timed_out = 1;
		}

		command = recGetCommand();
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

