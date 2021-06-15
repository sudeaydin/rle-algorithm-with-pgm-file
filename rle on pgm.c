//Sude Þevval Aydýn
//19011062
#include<stdio.h>
#include<stdlib.h>

typedef struct pgm{
	int maxVal;
    int width;
    int height;
    int data[800][800];
}PGM;

typedef struct aray{
	int array[500];
}Arr;

void read(PGM *imginfo);
Arr rle(PGM *imginfo);
void decode(Arr arr);
void islem(Arr arr);


int main(){
	
	PGM *imginfo = malloc(sizeof(PGM));
	read(imginfo);
	Arr  arr1 = rle(imginfo);
	decode(arr1);
	islem(arr1);


    return 0;
}

void read(PGM *imginfo){
	FILE *imagein;
    int row, col;
    int i,j;
    char infpath[500],outfpath[500];
    
    printf("PGM dosyasinin ismini giriniz :");
    scanf("%s",infpath);
    imagein = fopen(infpath,"rb");

    if(imagein == NULL){
        printf("Hata dosya acilamadi.");
        exit(0);
    }


    while(getc(imagein) != '\n');         

    if (getc(imagein) == '#' ) {
        while(getc(imagein) != '\n');
    }
    else{
        fseek(imagein, -1, SEEK_CUR);
    }

    fscanf(imagein,"%d", &imginfo->width);
    fscanf(imagein,"%d", &imginfo->height);
    fscanf(imagein,"%d", &imginfo->maxVal);
    printf("\nPGM dosyasinin : \n");
    printf("satir sayisi = %d\n",imginfo->height);
    printf("sutun sayisi= %d\n",imginfo->width);
    
    
    for (row=0; row<imginfo->height; row++){
        for (col=0; col < imginfo->width; col++){
            fscanf(imagein,"%d", &imginfo->data[row][col]);
             
        }
    }
  //  printf("\n");
    printf("PGM dosyasininin verisi : \n");
     for (row=0; row<imginfo->height; row++){
        for (col=0; col < imginfo->width; col++){
            printf("%d ",imginfo->data[row][col]);
        }
        printf("\n");
    }

    fclose(imagein);
}

Arr rle(PGM *imginfo){
	int i,j,k;
	Arr ar;
    int sifirfark=0;
	int toplam=0;
	int counter=1;
	int say=0;
	int r=2;
    ar.array[0]=imginfo->height;
    ar.array[1]=imginfo->width;
    
	for (i=0; i<imginfo->height;i++){
	    for (j=0;j< imginfo->width; j++){
	    	if( (j!=imginfo->width-1) && (imginfo->data[i][j] == imginfo->data[i][j+1])){
	    		counter++;
	    		
			}
			else{
				ar.array[r]=counter;
				r++;
				ar.array[r]=imginfo->data[i][j];
				r++;
				counter=1;
				
			}
			
        }
		counter=1;
    }
    ar.array[r]=0; //dizinin sonunu belirleyebilmek için
    
    i=0;
    int count=0;
   while(ar.array[i]!=0){
   	i++;
   	count++;
   }
    
    for(i=3;i<count;i=i+2){
    	if(ar.array[i]==ar.array[i+2] && ar.array[i]!=0 ){
    		
    		j=i;
    		
    		while(ar.array[j]==ar.array[j+2]){
    			toplam=ar.array[j-1]+toplam;
    			j=j+2;
    			say++;
    			
    			
			}
			toplam=ar.array[j-1]+toplam;
			ar.array[i-1]=toplam;
			
			
			for(k=i+1;k<count;k++){
				ar.array[k]=ar.array[j+1];
				j++;
			}
			
		
			toplam=0;
				
			
		}
    	
    	
	}
	
	printf("\nPGM dosyasinin encoded hali:\n");
	for(k=0;k<count-say*2;k++){
		printf("%d ",ar.array[k]);
	}
	
   

    FILE *fp;
	if ((fp = fopen ("test_encoded.txt", "w+")) == NULL) {
         printf("Dosya açma hatasý!");
         exit(1);
    }
    for (j=0;j<count-say*2;j++){
     	 fprintf(fp, "%d", ar.array[j]);
    	
	}
	printf("\nBu bilgi test_encoded.txt adiyla kaydedilmistir.\n");
	printf("\n");
	
	printf("\n---Kontrol Adimlari--- \n");
	printf("1)Encode edilmis goruntudeki piksel sayisinin SatirSayisi*SutunSayisi kadar oldugunun kontrolu \n");
	printf("PGM dosyasinin SatirSayisi*SutunSayisi degeri:  %d",ar.array[0]*ar.array[1]);
	
	for(i=2;i<count-say*2;i=i+2){
		toplam=toplam+ar.array[i];	
	}
	printf("\nPGM dosyasinin SatirSayisi*SutunSayisi degerinin encode edilmis veride hesaplanmasi:  %d\n",toplam);
	
	printf("2)Encode edilmis goruntude 0-255 arasindaki renklerin kullanildiginin kontrolu \n");
	
	i=3;
	int tmp=1;
	while(i<count-say*2 && tmp==1){
		if(ar.array[i]<0 || ar.array[i]>255){
			tmp=0;
		}
		i=i+2;	
	}
	if(tmp==0){
		printf("Renk araligi hatasi!!");
	}
	else{
		printf("Renk araligi uygun.\n");
	}
	
	printf("3)Ayni renge ait Run Length in arka arkaya bulunmadiginin kontrolu: \n");
	i=3;
	tmp=1;
	while(i<count-say*2 && tmp==1){
		if(ar.array[i]==ar.array[i+2]){
			tmp=0;
		}
		i=i+2;	
	}
	if(tmp==0){
		printf("Hata!!Ayni renge ait Run Length arka arkaya bulunuyor!!");
	}
	else{
		printf("Ayni renge ait Run Length arka arkaya bulunmuyor.");
	}
	 return ar;
	
}

void decode(Arr arr){
	int i,j;
	int len;
	int arr1[arr.array[0]][arr.array[1]];
	int realsize=arr.array[0]*arr.array[1];
	int arr2[realsize];
	i=0;
	while(arr.array[i]!=0){
		i++;
	}
	len=i;

	int k=0;
	j=0;
	
	for(i=2;i<len;i=i+2){
		while(j<arr.array[i] ){
			arr2[k]=arr.array[i+1];
			j++;
			k++;
		}
		j=0;
	}
	k=0;
	for(i=0;i<arr.array[0];i++){
		for(j=0;j<arr.array[1];j++){
			arr1[i][j]=arr2[k];
			k++;
		}
	}
	
   int h=arr.array[1];
   int w=arr.array[0];
   FILE* pgmimg;
   pgmimg = fopen("test_decoded.pgm", "wb"); 
   fprintf(pgmimg, "P2\n");
   
   fprintf(pgmimg, "%d %d\n", h, w); 
   fprintf(pgmimg, "255\n"); 
   for (i = 0; i <w; i++) {
      for (j = 0; j <h; j++) {
         fprintf(pgmimg, "%d ", arr1[i][j]); 
      }
      fprintf(pgmimg, "\n");
   }
   fclose(pgmimg);
   printf("\n");
   printf("\nPGM dosyasinin decode edilmis hali test_decoded.pgm  adiyla kaydedilmistir.\n");
   
}

void islem(Arr arr){
	int oldn,newn;
	int i,len,k,deger,j;
	int r,c;
	
	i=0;
	while(arr.array[i]!=0){
		i++;
	}
	len=i;
	
	printf("\nPGM dosyasinin encode edilmis hali icin yapilacak islemler :\n");
	printf("1) Verilen bir rengin degerini degistirme:\n");
	printf("Degistirmek istediginiz rengin kodunu giriniz: ");
	scanf("%d",&oldn);
	printf("Yeni rengin kodunu giriniz: ");
	scanf("%d",&newn);
	int arr1[len];

	for(i=0;i<len;i++){
		arr1[i]=arr.array[i];
	}
	for(i=3;i<len;i=i+2){
		if(arr1[i]==oldn){
			arr1[i]=newn;
		}
	}
	printf("RLE nin yeni hali : \n");
	for(i=0;i<len;i++){
		printf("%d ",arr1[i]);
	}
	
	FILE *fp;
	if ((fp = fopen ("test_encoded.txt", "a")) == NULL) {
         printf("Dosya açma hatasý!");
         exit(1);
    }
    fprintf(fp,"\n");
    for (i=0;i<len;i++){
     	 fprintf(fp, "%d", arr1[i]);
    	
	}
	printf("\nBu bilgi test_encoded.txt adiyla kaydedilmistir.\n");
	printf("\n");
	printf("2)Koordinatlari giris olarak verilen bir pikselin degerini degistirme\n");
	printf("Degistirmek istediginiz pikselin satir numarasini giriniz: ");
	scanf("%d",&r);
	printf("Degistirmek istediginiz pikselin sutun numarasini giriniz:  ");
	scanf("%d",&c);
	printf("Degistirmek istediginiz pikselin degerini giriniz:  ");
	scanf("%d",&deger);
	
	int sira=(r-1)*arr.array[1]+c;
	int toplam=0;
	i=2;
	int temp=0;
	j=0;
	 k=0;
	 int t;
	 int counter=1;
	 int lenn;
	int realsize=arr.array[0]*arr.array[1];
	int arr2[realsize];
	int arr3[100];
	int arr4[100];
	
	while(i<len && temp==0){
		if(toplam<sira){
			toplam=toplam+arr.array[i];
		}
		else{
			for(t=2;t<i;t=t+2){
		       while(j<arr.array[t] ){
			     arr2[k]=arr.array[t+1];
		     	j++;
		    	k++;
		       }
		    j=0;
	       }
	       
	      
	       for(t=toplam-1;t>=sira-1;t--){
	       	arr2[t+1]=arr2[t];
	      
		   }
		 
	       
	      arr2[sira-1]=deger;

		   
	     t=0;
	     for (j=0;j<toplam+1; j++){
	    	if( arr2[j] == arr2[j+1]){
	    		counter++;
	    		
			}
			else{
				arr3[t]=counter;
				t++;
				arr3[t]=arr2[j];
				t++;
				counter=1;
				
			}
			
        }
       
		
		arr3[t]=0;
		t=0;
		j=0;
		 while(arr3[t]!=0){
            t++;
        	j++;
        }
        lenn=j;
        
		arr4[0]=arr.array[0];
		arr4[1]=arr.array[1];
		k=0;
		
         for(t=2;t<lenn+2;t++){
	       	arr4[t]=arr3[k];
	       	k++;
		}
		
		k=i;
		 for(j=t;j<lenn+len;j++){
		 	arr4[j]=arr.array[k];
		 	k++;
        	
		}	
			temp=1;
			
		}
		i=i+2;
	}
	printf("RLE nin yeni hali:\n");
	 for(j=0;j<len+(lenn-(i-4));j++){
		 	printf("%d ",arr4[j]);
	}
	FILE *fp2;
	if ((fp2 = fopen ("test_encoded.txt", "a")) == NULL) {
         printf("Dosya açma hatasý!");
         exit(1);
    }
    fprintf(fp2,"\n");

    for (j=0;j<len+(lenn-(i-4));j++){
     	 fprintf(fp2, "%d", arr4[j]);
    	
	}
	printf("\nBu bilgi test_encoded.txt adiyla kaydedilmistir.\n");
	printf("\n");	
		
	

	
	
	printf("\n");		
	printf("\n3)Resmin histograminin hesaplanmasi");		
    int hist[256];
    int sifirfark=0;
    toplam=0;
	for (i=0; i<256; i++){
        hist[i]=0;   
    }
    
    for (i=3; i<len; i=i+2){
    	 for (j=0;j<arr.array[i-1]; j++){
	    	hist[arr.array[i]]=hist[arr.array[i]]+1;
			}	
    
           
    }
  
    for (j=0;j<256; j++){
    	if(hist[j]!=0){
    		sifirfark++;
	}
             
    }
    int number= sifirfark*2+2;
    int final[arr.array[1]*arr.array[0]*2+2];
    final[0]=arr.array[0];
    final[1]=arr.array[1];
     r=2;
    
    for (j=0;j<256; j++){
    	if(hist[j]!=0){
    		final[r]=hist[j];
    		r++;
    		final[r]=j;
    		r++;
				
    	}      
    }
    
    printf("\nPGM dosyasinin histogram hali:\n");
    for (j=0;j<number;j++){
     	printf("%d ",final[j]);
	}
	FILE *fp1;
	if ((fp1 = fopen ("test_encoded.txt", "a")) == NULL) {
         printf("Dosya açma hatasý!");
         exit(1);
    }
    fprintf(fp1,"\n");
    for (i=0;i<number;i++){
     	 fprintf(fp1, "%d", final[i]);
    	
	}
	printf("\nBu bilgi test_encoded.txt adiyla kaydedilmistir.\n");
	printf("\n");	
	
	

	
}	
	

	
	
	
	

