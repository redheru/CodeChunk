#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#include "GL/glew.h"
#include "GL/glut.h"

#include "glsl.h"

/*
** シェーダーのソースプログラムをメモリに読み込む
*/
int readShaderSource(GLuint shader, const char *file)
{
  FILE *fp;
  const GLchar *source;
  GLsizei length;
  int ret;
  
  /* ファイルを開く */
  fp = fopen(file, "rb");
  if (fp == NULL) {
    perror(file);
    return -1;
  }
  
  /* ファイルの末尾に移動し現在位置（つまりファイルサイズ）を得る */
  fseek(fp, 0L, SEEK_END);
  length = ftell(fp);
  
  /* ファイルサイズのメモリを確保 */
  source = (GLchar *)malloc(length);
  if (source == NULL) {
    fprintf(stderr, "Could not allocate read buffer.\n");
    return -1;
  }
  
  /* ファイルを先頭から読み込む */
  fseek(fp, 0L, SEEK_SET);
  ret = fread((void *)source, 1, length, fp) != (size_t)length;
  fclose(fp);
  
  /* シェーダのソースプログラムのシェーダオブジェクトへの読み込み */
  if (ret)
    fprintf(stderr, "Could not read file: %s.\n", file);
  else
    glShaderSource(shader, 1, &source, &length);
  
  /* 確保したメモリの開放 */
  free((void *)source);
  
  return ret;
}

/*
** シェーダの情報を表示する
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
** プログラムの情報を表示する
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
