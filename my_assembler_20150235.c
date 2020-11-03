/*
 * ȭ�ϸ� : my_assembler_00000000.c 
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

/*
 *
 * ���α׷��� ����� �����Ѵ�. 
 *
 */
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20150235.h"
/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ���� 
 * ��ȯ : ���� = 0, ���� = < 0 
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�. 
 *		   ���� �߰������� �������� �ʴ´�. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[])
{
	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if (assem_pass1() < 0)
	{
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}
	make_opcode_output("output_20150235.txt");
	/*
	* ���� ������Ʈ���� ���Ǵ� �κ�
	
	make_symtab_output("symtab_20150235");
	if(assem_pass2() < 0 ){
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n") ; 
		return -1 ; 
	}

	make_objectcode_output("output_20150235") ; 
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�. 
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ� 
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���. 
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
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)�� 
 *        �����ϴ� �Լ��̴�. 
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *	
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char *inst_file)
{	/* add your code here */
	FILE *file = fopen(inst_file, "r");
    int errno;
    int i = 0;
    while (!feof(file)) { //inst.txt���� opcode ������ ���� �� �޾ƿ� �׸񺰷� ����
        inst_table[i] = (inst *)malloc(sizeof(inst));
        fscanf(file, "%s %d %s %d", &inst_table[i]->opName, &inst_table[i]->format1, &inst_table[i]->opcode, &inst_table[i]->numop); 
        i++;
        inst_index++;
    }
    fclose(file);
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�. 
 * �Ű� : ������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0  
 * ���� : ���δ����� �����Ѵ�.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{    /* add your code here */
    FILE* file = fopen(input_file, "r");
    int errno;
    int i = 0;
    char buff[200]; //�ҽ��ڵ� ������ �޾ƿ��� ���ؼ� ����
    int len;
    while (!feof(file)) {
        if (NULL == fgets(buff, 200, file)) // �ҽ��ڵ� ���� �޾ƿ�
            break;
        buff[strlen(buff) - 1] = '\0';
        len = (int)strlen(buff);
        input_data[i] = (char*)malloc(sizeof(char) * len + 1); // input_data�� �ҽ��ڵ��� ���� ��ŭ �Ҵ�
        strcpy(input_data[i], buff);                        // buff�� ������ input_data�� ����
        i++;
        line_num++;
    }
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�. 
 *        �н� 1�� ���� ȣ��ȴ�. 
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�  
 * ��ȯ : �������� = 0 , ���� < 0 
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
    int errno;
	/* add your code here */
    token_table[token_line] = (token*)malloc(sizeof(token));
    for (int j = 0; j < MAX_OPERAND; j++)
        token_table[token_line]->operand[j] = NULL;
    if (str[0] == '\t' || str[0] == ' '){ // label�� ���� ���
        token_table[token_line]->label = "";
        token_table[token_line]->operator = strtok(str, "\t");
        token_table[token_line]->operand[2] = strtok(NULL, "\t");
        token_table[token_line]->comment = strtok(NULL, "\n");
    }
    else if (str[0] == '.') { // label�� ������ "."�϶�
        if (str[6] == ' ') { //comment�� ���� ��
            token_table[token_line]->label = strtok(str, "\n");
            token_table[token_line]->comment = NULL;
        }
        else{ //comment�� ������
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
    if (token_table[token_line]->operand[2] != NULL) {   //operand�� �������� ��� ������ ������
        token_table[token_line]->operand[0] = strtok(token_table[token_line]->operand[2], ",");
        token_table[token_line]->operand[1] = strtok(NULL, ",");            // ������ ������ NULL�� �����
        token_table[token_line]->operand[2] = strtok(NULL, "\n");
    }
    token_line++;
    return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�. 
 * �Ű� : ��ū ������ ���е� ���ڿ� 
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0 
 * ���� : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str)
{
     if (str == NULL)
        return 0;
    if (str[0] == '+') { //op�ڵ带 ���ϱ� ���� �����͸� �ϳ� �ڷ� �Űܼ� '+'�κ��� ������
        str = str + 1;
    }
    for (int i = 0; i < inst_index; i++){ // inst_table�� ������ ���� �� ���� �ݺ�
        if (strcmp(inst_table[i]->opName, str) == 0) { // �̸��� ������ �ε��� ��ȣ�� ����
            return i;
        }
    }
    return -1;
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	/* add your code here */
	/* input_data�� ���ڿ��� ���پ� �Է� �޾Ƽ� 
	 * token_parsing()�� ȣ���Ͽ� token_unit�� ����
	 */
    int errno;
    for (int i = 0; i < line_num; i++) { //input_data�� �ϳ��� �Ѱ��־ token_table ����
        token_parsing(input_data[i]);    
    }

    return errno;
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 4��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char* file_name)
{
    FILE* file = fopen(file_name, "w");
     for(int i = 0; i < line_num-1; i++){   //�ҽ��ڵ��� ���� ���� ��ŭ �ݺ�
         int k = search_opcode(token_table[i]->operator);       //operator �κ��� inst_table �ȿ� �ִ��� Ȯ��
        if (strcmp(token_table[i]->label, ".") == 0) {          //label�κ��� '.'�̸� �з�
            fprintf(file, "%s\t", token_table[i]->label);
            if(token_table[i]->comment != NULL)
                fprintf(file, "%s\n", token_table[i]->comment);
            else
                fprintf(file, "\n");
        }
        else if (k >= 0) { //inst_table �ȿ� ������
            if (inst_table[k]->numop == 0)          //opcode�� ������ oparand�� ������ 0�� ��� �з�
                fprintf(file, "%s\t%s\t\t", token_table[i]->label, token_table[i]->operator);
            else {
                fprintf(file, "%s\t%s\t", token_table[i]->label, token_table[i]->operator);
                fprintf(file, "%s", token_table[i]->operand[0]);
                if (token_table[i]->operand[1] != NULL) {               //operand�� �������� ��� Ȯ���ϸ鼭 �߰� ���
                    fprintf(file, ",%s", token_table[i]->operand[1]);
                    if (token_table[i]->operand[2] != NULL)
                        fprintf(file, ",%s", token_table[i]->operand[2]);
                }
                fprintf(file, "\t");
            }
            fprintf(file, "%s\n", inst_table[k]->opcode);
        }
        else {              //inst_table �ȿ� ���� ���
            fprintf(file, "%s\t%s\t", token_table[i]->label, token_table[i]->operator);
            if (token_table[i]->operand[0] == NULL)
                fprintf(file, "\n");
            else {          //operand�� ������ ��� 
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
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ SYMBOL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_symtab_output(char *file_name)
{
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ LITERAL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_literaltab_output(char *filen_ame)
{
	/* add your code here */
}

/* --------------------------------------------------------------------------------*
* ------------------------- ���� ������Ʈ���� ����� �Լ� --------------------------*
* --------------------------------------------------------------------------------*/

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */
}
