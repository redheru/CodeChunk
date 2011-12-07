#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "GL/glew.h"
#include "GL/glut.h"

#include "glsl.h"

/*
** �V�F�[�_�[�̃\�[�X�v���O�������������ɓǂݍ���
*/
int readShaderSource(GLuint shader, const char *file)
{
  FILE *fp;
  const GLchar *source;
  GLsizei length;
  int ret;
  
  /* �t�@�C�����J�� */
  fp = fopen(file, "rb");
  if (fp == NULL) {
    perror(file);
    return -1;
  }
  
  /* �t�@�C���̖����Ɉړ������݈ʒu�i�܂�t�@�C���T�C�Y�j�𓾂� */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);
  
  /* �t�@�C���T�C�Y�̃��������m�� */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
    fprintf(stderr, "Could not allocate read buffer.\n");
    return -1;
  }
  
  /* �t�@�C����擪����ǂݍ��� */
  fseek(fp, 0L, SEEK_SET);
  ret = fread((void *)source, 1, length, fp) != (size_t)length;
  fclose(fp);
  
  /* �V�F�[�_�̃\�[�X�v���O�����̃V�F�[�_�I�u�W�F�N�g�ւ̓ǂݍ��� */
  if (ret)
    fprintf(stderr, "Could not read file: %s.\n", file);
  else
    glShaderSource(shader, 1, &source, &length);
  
  /* �m�ۂ����������̊J�� */
  free((void *)source);
  
  return ret;
}

/*
** �V�F�[�_�̏���\������
*/
void printShaderInfoLog(GLuint shader)
{
  GLsizei bufSize;
  
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog;
    
    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;
      
      glGetShaderInfoLog(shader, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}

/*
** �v���O�����̏���\������
*/
void printProgramInfoLog(GLuint program)
{
  GLsizei bufSize;
  
  glGetProgramiv(program, GL_INFO_LOG_LENGTH , &bufSize);
  
  if (bufSize > 1) {
    GLchar *infoLog;
    
    infoLog = (GLchar *)malloc(bufSize);
    if (infoLog != NULL) {
      GLsizei length;
      
      glGetProgramInfoLog(program, bufSize, &length, infoLog);
      fprintf(stderr, "InfoLog:\n%s\n\n", infoLog);
      free(infoLog);
    }
    else
      fprintf(stderr, "Could not allocate InfoLog buffer.\n");
  }
}
