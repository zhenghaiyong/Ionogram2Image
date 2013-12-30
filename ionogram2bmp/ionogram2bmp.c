#include <stdio.h>
#include <stdlib.h>

#define CAST(Class, obj) ((Class)(obj))

int main(int argc, char *argv[])
{
  int threshold = 0; /* 阀值 */
	
  unsigned char * arr0 = 0; /* Matrix0的存储 */
  int row0 = 0;
  int col0 = 0;
	
  /* Matrix10、Matrix11、Matrix12、Matrix13、Matrix14、Matrix15、Matrix16的存储 */
  unsigned char * arr1 = 0;
  /*int row1 = 0;*/
  int col1 = 0;
	
  /* Matrix20、Matrix21、Matrix22、Matrix23、Matrix24、Matrix25、Matrix26的存储 */
  unsigned char * arr2 = 0;
  /*int row2 = 0;*/
  int col2 = 0;
	
  unsigned char * arr3 = 0; /* Matrix30的存储 */
  int row3 = 0;
  int col3 = 0;
	
  FILE * input = 0;
  int inputLen = 0;
    
  FILE * output = 0;

  if( argc != 4 )
    {
      fprintf(stderr, "Usage:\t %s [input_file] [output_bmp_name] [threshold]\n", argv[0]);
      return 7;
    }

  if (!(input=fopen(argv[1],"rb"))) 
    {
      printf("Cannot open file %s.\n", argv[1]);
      exit(7);
    }
	
  fseek(input, 0, SEEK_END); //移动到文件的末端以便于下一步知道文件字节数
  inputLen = ftell(input); //得到文件的字节数
  rewind(input); //使指针返回到文件的开始
	
  /*
   * 因为从文件filePath中一个字节和162X1280的矩阵Matrix0的每个元素一一对应，
   * 并且每162个字节形成一列，
   * 并且读文件input写入arr0时，使arr0[index]和文件input的index位置数据一一对应，	 	  
   * 所以arr0[row0+col0*162]和162X1280矩阵Matrix0的row0行col0列的元素一一对应，
   */
  arr0 = CAST(unsigned char *, malloc(inputLen));
  fread(arr0, sizeof(unsigned char), inputLen, input);
  fclose(input);
	
  /*
   * 取162X1280的矩阵Matrix0的列首为0XFF的列，构成162X640矩阵Matrix10。
   * arr1[row1+col1*162]和162X640矩阵Matrix10的row1行col1列的元素一一对应，
   */
  arr1 = CAST(unsigned char *, malloc(162*640));

  /*
   * 取162X1280的矩阵Matrix0的列首为0XEE的列，构成162X640矩阵Matrix20。
   * arr2[row2+col2*162]和162X640矩阵Matrix20的row2行col2列的元素一一对应，
   * 其中row2>=0&&row2<162，col2>=0&&col2<640。
   */
  arr2 = CAST(unsigned char *, malloc(162*640));

  for(col0=0; col0<1280; ++col0)
    if(arr0[col0*162] == 0XFF) /*row0为0*/
      {
        for(row0=0; row0<162; ++row0)
          arr1[row0+col1*162] = arr0[row0+col0*162]; /*row1等于row0*/
        ++col1;
      }
    else if(arr0[col0*162] == 0XEE)
      {
        for(row0=0; row0<162; ++row0)
          arr2[row0+col2*162] = arr0[row0+col0*162]; /*row2等于row0*/
        ++col2;
      }
  
  free(arr0); /*之后用不到arr0了，释放*/
  
  /*
   * 162X640矩阵Matrix10去掉前2行后，变成160X640矩阵Matrix11。
   * arr1[row1+2+col1*162]和160X640矩阵Matrix11的row1行col1列的元素一一对应，
   */
  
  /*
   * 160X640矩阵Matrix11垂直翻转后，变成160X640矩阵Matrix12。
   * arr1[161-row1+col1*162]和160X640矩阵Matrix12的row1行col1列的元素一一对应，
   */
  
  /*
   * 取160X640矩阵Matrix12的每个元素的高4位构成160X640矩阵Matrix13。
   * (0XF0&arr1[161-row1+col1*162])>>0X4和160X640矩阵Matrix13的row1行col1列的元素一一对应，
   */
  
  /*
   * 取160X640矩阵Matrix13的每个元素的低4位构成160X640矩阵Matrix14。
   * 0X0F&arr1[161-row1+col1*162]和160X640矩阵Matrix14的row1行col1列的元素一一对应，
   */
  
  /*
   * 把160X640矩阵Matrix13和160X640矩阵Matrix14以插值的形式构成320X640矩阵Matrix15。
   * (row1&0X01)?(0X0F&arr1[161-(row1>>1)+col1*162]):((0XF0&arr1[161-(row1>>1)+col1*162])>>0X4))
   * 和320X640矩阵Matrix15的row1行col1列的元素一一对应，
   */
  
  /*
   * 对320X640矩阵Matrix15进行阀值为threshold阈值分割构成320X640矩阵Matrix16。
   * 阀值分割：大于阀值为1，小于阀值为0。
   * ((row1&0X01)?(0X0F&arr1[161-(row1>>1)+col1*162]):((0XF0&arr1[161-(row1>>1)+col1*162])>>0X4))>threshold?1:0
   * 和320X640矩阵Matrix16的row1行col1列的元素一一对应，
   */
  
  /*
   * 同理，构成矩阵Matrix21、Matrix22、Matrix23、Matrix24、Matrix25、Matrix26。
   */
  
  /*
   * 将320X640矩阵Matrix16和320X640矩阵Matrix26对应行列的元素相加，
   * 进行阀值为1的阀值分隔，构成320X640矩阵Matrix30
   * arr3[row3+col3*320]和320X640矩阵Matrix30的row3行col3列的元素一一对应，
   */

  arr3 = CAST(unsigned char *, malloc(320*640));

  threshold = atoi(argv[3]);
  for(row3=0;row3<320;++row3)
    for(col3=0;col3<640;++col3)
      arr3[row3+col3*320] = 
        (((row3&0X01)?(0X0F&arr1[161-(row3>>1)+col3*162]):((0XF0&arr1[161-(row3>>1)+col3*162])>>0X4))>threshold?1:0
         + ((row3&0X01)?(0X0F&arr2[161-(row3>>1)+col3*162]):((0XF0&arr2[161-(row3>>1)+col3*162])>>0X4))>threshold?1:0)
        >0?0X01:0X00;
  
  free(arr1);
  free(arr2);

  /* write raw  
  output = fopen(argv[2], "wb");

  for(row3=0; row3<320; ++row3)
    for(col3=0; col3<640; ++col3)
      fwrite(&arr3[row3+col3*320], sizeof(unsigned char), 1, output);
  
  free(arr3);
  
  fclose(output);
  */

  write_binary_bmp(argv[2], 640, 320, arr3);
  
  return 0;
}
