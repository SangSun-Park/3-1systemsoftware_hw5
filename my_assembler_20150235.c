/*
 * 화일명 : my_assembler_00000000.c 
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 * 파일 내에서 사용되는 문자열 "00000000"에는 자신의 학번을 기입한다.
 */

/*
 *
 * 프로그램의 헤더를 정의한다. 
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// 파일명의 "00000000"은 자신의 학번으로 변경할 것.
#include "my_assembler_20150235.h"
/* ----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[])
{
	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}

	if (assem_pass1() < 0)
	{
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}
	make_opcode_output("output_20150235.txt");
	/*
	* 추후 프로젝트에서 사용되는 부분
	
	make_symtab_output("symtab_20150235");
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n") ; 
		return -1 ; 
	}

	make_objectcode_output("output_20150235") ; 
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
	int result;

	if ((result = init_inst_file("inst.txt")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 자유롭게 구현한다. 예시는 다음과 같다.
 *	
 *	===============================================================================
 *		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char *inst_file)
{	/* add your code here */
	FILE *file = fopen(inst_file, "r");
    int errno;
    int i = 0;
    while (!feof(file)) { //inst.txt에서 opcode 정보를 한줄 씩 받아와 항목별로 저장
        inst_table[i] = (inst *)malloc(sizeof(inst));
        fscanf(file, "%s %d %s %d", &inst_table[i]->opName, &inst_table[i]->format1, &inst_table[i]->opcode, &inst_table[i]->numop); 
        i++;
        inst_index++;
    }
    fclose(file);
	return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 어셈블리 할 소스코드를 읽어 소스코드 테이블(input_data)를 생성하는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 라인단위로 저장한다.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{    /* add your code here */
    FILE* file = fopen(input_file, "r");
    int errno;
    int i = 0;
    char buff[200]; //소스코드 한줄을 받아오기 위해서 생성
    int len;
    while (!feof(file)) {
        if (NULL == fgets(buff, 200, file)) // 소스코드 한줄 받아옴
            break;
        buff[strlen(buff) - 1] = '\0';
        len = (int)strlen(buff);
        input_data[i] = (char*)malloc(sizeof(char) * len + 1); // input_data에 소스코드의 길이 만큼 할당
        strcpy(input_data[i], buff);                        // buff의 내용을 input_data에 저장
        i++;
        line_num++;
    }
	return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 파싱을 원하는 문자열  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
    int errno;
	/* add your code here */
    token_table[token_line] = (token*)malloc(sizeof(token));
    for (int j = 0; j < MAX_OPERAND; j++)
        token_table[token_line]->operand[j] = NULL;
    if (str[0] == '\t' || str[0] == ' '){ // label이 없는 경우
        token_table[token_line]->label = "";
        token_table[token_line]->operator = strtok(str, "\t");
        token_table[token_line]->operand[2] = strtok(NULL, "\t");
        token_table[token_line]->comment = strtok(NULL, "\n");
    }
    else if (str[0] == '.') { // label이 있으나 "."일때
        if (str[6] == ' ') { //comment가 없을 때
            token_table[token_line]->label = strtok(str, "\n");
            token_table[token_line]->comment = NULL;
        }
        else{ //comment가 있을때
            token_table[token_line]->label = strtok(str, "\t");
            token_table[token_line]->comment = strtok(NULL, "\n");
        }
        token_table[token_line]->operator = NULL;
        token_table[token_line]->operand[2] = NULL;
    }
    else {
        token_table[token_line]->label = strtok(str, "\t");
        token_table[token_line]->operator = strtok(NULL, "\t");
        token_table[token_line]->operand[2] = strtok(NULL, "\t");
        token_table[token_line]->comment = strtok(NULL, "\n");
    }
    if (token_table[token_line]->operand[2] != NULL) {   //operand가 여려개일 경우 때문에 동작함
        token_table[token_line]->operand[0] = strtok(token_table[token_line]->operand[2], ",");
        token_table[token_line]->operand[1] = strtok(NULL, ",");            // 내용이 없으면 NULL로 저장됨
        token_table[token_line]->operand[2] = strtok(NULL, "\n");
    }
    token_line++;
    return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str)
{
     if (str == NULL)
        return 0;
    if (str[0] == '+') { //op코드를 비교하기 위해 포인터를 하나 뒤로 옮겨서 '+'부분을 없에줌
        str = str + 1;
    }
    for (int i = 0; i < inst_index; i++){ // inst_table에 내용이 없을 때 까지 반복
        if (strcmp(inst_table[i]->opName, str) == 0) { // 이름이 같으면 인덱스 번호를 리턴
            return i;
        }
    }
    return -1;
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	/* add your code here */
	/* input_data의 문자열을 한줄씩 입력 받아서 
	 * token_parsing()을 호출하여 token_unit에 저장
	 */
    int errno;
    for (int i = 0; i < line_num; i++) { //input_data를 하나씩 넘겨주어서 token_table 생성
        token_parsing(input_data[i]);    
    }

    return errno;
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 명령어 옆에 OPCODE가 기록된 표(과제 4번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*        또한 과제 4번에서만 쓰이는 함수이므로 이후의 프로젝트에서는 사용되지 않는다.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char* file_name)
{
    FILE* file = fopen(file_name, "w");
     for(int i = 0; i < line_num-1; i++){   //소스코드의 줄의 개수 만큼 반복
         int k = search_opcode(token_table[i]->operator);       //operator 부분이 inst_table 안에 있는지 확인
        if (strcmp(token_table[i]->label, ".") == 0) {          //label부분이 '.'이면 분류
            fprintf(file, "%s\t", token_table[i]->label);
            if(token_table[i]->comment != NULL)
                fprintf(file, "%s\n", token_table[i]->comment);
            else
                fprintf(file, "\n");
        }
        else if (k >= 0) { //inst_table 안에 있으면
            if (inst_table[k]->numop == 0)          //opcode가 있으나 oparand의 개수가 0일 경우 분류
                fprintf(file, "%s\t%s\t\t", token_table[i]->label, token_table[i]->operator);
            else {
                fprintf(file, "%s\t%s\t", token_table[i]->label, token_table[i]->operator);
                fprintf(file, "%s", token_table[i]->operand[0]);
                if (token_table[i]->operand[1] != NULL) {               //operand가 여러개일 경우 확인하면서 추가 출력
                    fprintf(file, ",%s", token_table[i]->operand[1]);
                    if (token_table[i]->operand[2] != NULL)
                        fprintf(file, ",%s", token_table[i]->operand[2]);
                }
                fprintf(file, "\t");
            }
            fprintf(file, "%s\n", inst_table[k]->opcode);
        }
        else {              //inst_table 안에 없을 경우
            fprintf(file, "%s\t%s\t", token_table[i]->label, token_table[i]->operator);
            if (token_table[i]->operand[0] == NULL)
                fprintf(file, "\n");
            else {          //operand가 존재할 경우 
                fprintf(file, "%s", token_table[i]->operand[0]);
                if (token_table[i]->operand[1] != NULL) {
                    fprintf(file, ",%s", token_table[i]->operand[1]);
                    if(token_table[i]->operand[2] != NULL)
                        fprintf(file, ",%s", token_table[i]->operand[2]);
                }
                fprintf(file, "\n");
            }
        }
    }
    fclose(file);
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 SYMBOL별 주소값이 저장된 TABLE이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_symtab_output(char *file_name)
{
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 LITERAL별 주소값이 저장된 TABLE이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_literaltab_output(char *filen_ame)
{
	/* add your code here */
}

/* --------------------------------------------------------------------------------*
* ------------------------- 추후 프로젝트에서 사용할 함수 --------------------------*
* --------------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 object code (프로젝트 1번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */
}
