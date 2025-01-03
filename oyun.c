#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "grafik.h"
#include <curl/curl.h>
typedef struct{
  char birimler[4][100];
  int  birimsayisi[4];
  char kahramanlar[2][100];
  char canavarlar[1][100];
  char arastirmalar[1][100];
  int arastirmaseviyesi;
  float  toplamsaldiri;
  float toplamsavunma;
}human;
typedef struct{
  char birimler[4][100];
  int  birimsayisi[4];
  char kahramanlar[2][100];
  char canavarlar[1][100];
  char arastirmalar[1][100];
  int arastirmaseviyesi;
  float  toplamsaldiri;
  float  toplamsavunma;
}ork_;
typedef struct{
  char irk[10];
  char ad[25];
  int saldiri;
  int savunma;
  int saglik;
  float kritiksans;
  struct unit* next;
}unit;
typedef struct{
  char irk[10];
  char ad[40];
  char bonusturu[20];
  char etkiledigibirim[30];
  float bonusdegeri;
  struct hero* next;
}hero;
typedef struct{
  char irk[30];
  char ad[30];
  char etkituru[20];
  char etkiledigibirim[30];
  float etkidegeri;
}creature;
typedef struct{
    char ad[30];
    char bonusturu[30];
    char etkiledigibirim[30];
    float degerler[3];
}research;
struct MemoryStruct {
    char* memory;
    size_t size;
};
void UnitJsonCek(char* array,unit* units);
void unitjson(char arr[][50],int* tempj,int* tempk,unit* units);
void UnitJsonAta(char* array,unit* units);
void HeroJsonCek(char* array,hero* heroes);
void herojson(char arr[][90],int* tempj,int* tempk,hero* heroes);
void HeroJsonAta(char* array,hero* heroes);
void CreatureJsonCek(char* array,creature* creatures);
void creaturejson(char arr[][100],int* tempj,int* tempk,creature* creatures);
void CreatureJsonAta(char* array,creature* creatures);
void ResearchJsonCek(char* array,research* researches);
void researchjson(char arr[][90],int* tempj,int* tempk,int* templ,research* researches);
void ResearchJsonAta(char* array,research* researches);
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
char* fetch_json_data(const char* url);
void SenaryoJsonCek(char* array,human* insan,ork_* ork,unit* units,hero* heroes,creature* creatures,research* researches);
void SenaryoJsonAta(char* array,human* insan,ork_* ork,unit* units,hero* heroes,creature* creatures,research* researches);
void karakterKaldir(char *str, char karakter);
void basBoslukKaldir(char *str);
void StatlariHesapla(unit* units,human* insan,ork_* ork,int adim);
void KahramanEtkisiniEkle(unit* units,human* insan,ork_* ork,hero* heroes);
void CanavarEtkisiniEkle(unit* units,human* insan,ork_* ork,creature* creatures);
void ArastirmaEtkisiniEkle(unit* units,human* insan,ork_* ork,research* researches);
void YorgunlukEtkisiniEkle(unit* units,human* insan,ork_* ork);
int KritikVurusHesapla(int unitsaldiri,float unitkritik,int adim);
float NetHasarHesapla(float toplamsaldiri,float toplamsavunma);
void insanOrantiliHasarDagilimi(unit* units,human* insan,int nethasar,float toplamsavunma);
void OrkOrantiliHasarDagilimi(unit* units,ork_* ork,int nethasar,float toplamsavunma);
void AdimlariMetneYazmak(unit* units,human* insan,ork_* ork,int adim,float toplamsaldiri,float toplamsavunma,float nethasar);
void GrafikCagir(unit* units,hero* heroes,creature* creature,human* insan,ork_* ork);
int main(){
    unit* units = (unit*)malloc(8 * sizeof(unit));
    //unit* units = malloc(sizeof(unit));
    hero* heroes = (hero*)malloc(9 * sizeof(hero));
    creature* creatures = (creature*)malloc(11 * sizeof(creature));
    research* researches = (research*)malloc(4 * sizeof(research));
    human* insan = (human*)malloc(sizeof(human));
    ork_* ork = (ork_*)malloc(sizeof(ork_));
    char* array = (char*)malloc(2500 * sizeof(char));
    for(int i=0;i<4;i++){
        insan->birimsayisi[i]=0;
        ork->birimsayisi[i]=0;
    }
    insan->toplamsaldiri=0;
    insan->toplamsavunma=0;
    ork->toplamsaldiri=0;
    ork->toplamsavunma=0;
    UnitJsonCek(array,units);
    HeroJsonCek(array,heroes);
    CreatureJsonCek(array,creatures);
    ResearchJsonCek(array,researches);
    SenaryoJsonCek(array,insan,ork,units,heroes,creatures,researches);
    GrafikCagir(units,heroes,creatures,insan,ork);
    KahramanEtkisiniEkle(units,insan,ork,heroes);
    CanavarEtkisiniEkle(units,insan,ork,creatures);
    ArastirmaEtkisiniEkle(units,insan,ork,researches);
    float nethasar=0;
    int adim=1,toplaminsanbirimsayisi=0,toplamorkbirimsayisi=0;
    StatlariHesapla(units,insan,ork,adim);
    AdimlariMetneYazmak(units,insan,ork,adim-1,ork->toplamsaldiri,insan->toplamsavunma,nethasar);
    do{
      toplaminsanbirimsayisi=0;
      toplamorkbirimsayisi=0;
      if(adim%5==0)
        YorgunlukEtkisiniEkle(units,insan,ork);
      if(adim%2==1){ //Adim sayisi tek iken insan saldýrýr
         nethasar = NetHasarHesapla(insan->toplamsaldiri,ork->toplamsavunma);
         OrkOrantiliHasarDagilimi(units,ork,nethasar,ork->toplamsavunma);
         StatlariHesapla(units,insan,ork,adim);
         AdimlariMetneYazmak(units,insan,ork,adim,insan->toplamsaldiri,ork->toplamsavunma,nethasar);
      }
      else{ //Adim sayisi çift iken orklar saldirir
         nethasar = NetHasarHesapla(ork->toplamsaldiri,insan->toplamsavunma);
         insanOrantiliHasarDagilimi(units,insan,nethasar,insan->toplamsavunma);
         StatlariHesapla(units,insan,ork,adim);
         AdimlariMetneYazmak(units,insan,ork,adim,ork->toplamsaldiri,insan->toplamsavunma,nethasar);
      }
      for(int i=0;i<4;i++){
        toplaminsanbirimsayisi += insan->birimsayisi[i];
        toplamorkbirimsayisi += ork->birimsayisi[i];
      }
      adim++;
    }while(toplaminsanbirimsayisi!=0 && toplamorkbirimsayisi!=0);
    GrafikCagir(units,heroes,creatures,insan,ork);
    free(units);
    free(heroes);
    free(creatures);
    free(researches);
    free(insan);
    free(ork);
    free(array);
    return 0;
}
void UnitJsonCek(char* array,unit* units){
    FILE* file;
    file=fopen("Files\\unit_types.json","r");
    if(file==NULL){
        printf("Dosya 'Files/unit_types.json' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.");
    }
    else{
        fread(array,sizeof(char),1248,file);
    }
    fclose(file);
    array[strlen(array)-1]='\0';
    UnitJsonAta(array,units);
}
void unitjson(char arr[][50],int* tempj,int* tempk,unit* units) {
    int j = *tempj;
    int k = *tempk;
    j++;
    if(strstr(arr[j],"saldiri")!=NULL)
        sscanf(arr[j++],"saldiri: %d",&(units[k].saldiri));
    if(strstr(arr[j],"savunma")!=NULL)
        sscanf(arr[j++],"savunma: %d",&(units[k].savunma));
    if(strstr(arr[j],"saglik")!=NULL)
        sscanf(arr[j++],"saglik: %d",&(units[k].saglik));
    if(strstr(arr[j],"kritik_sans")!=NULL)
        sscanf(arr[j++],"kritik_sans: %f",&(units[k].kritiksans));
    k++;
    *tempj = j;
    *tempk = k;
}
void UnitJsonAta(char* array,unit* units){
    int i=0,j=0;
    char arr[52][50];
    char* deneme=strtok(array,"\n");
    while(deneme!=NULL){
        basBoslukKaldir(deneme);
        karakterKaldir(deneme,'"');
        strcpy(arr[i++],deneme);
        deneme=strtok(NULL,"\n");
        if(deneme==NULL)
           break;
    }
    int k=0;
    while(j<i){
        if(strstr(arr[j],"insan_imparatorlugu") != NULL){
            do{
                strcpy(units[k].irk,"insan");
                if(strstr(arr[j],"piyadeler")!=NULL){
                    strcpy(units[k].ad,"piyadeler");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"okcular")!=NULL){
                    strcpy(units[k].ad,"okcular");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"suvariler")!=NULL){
                    strcpy(units[k].ad,"suvariler");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"kusatma_makineleri")!=NULL){
                    strcpy(units[k].ad,"kusatma_makineleri");
                    unitjson(arr,&j,&k,units);
                }
                j++;
            }while(strstr(arr[j],"ork_legi")==NULL && j<i);
        }
        if(strstr(arr[j], "ork_legi") != NULL){
            j++;
            do{
                strcpy(units[k].irk,"ork");
                if(strstr(arr[j],"ork_dovusculeri")!=NULL){
                    strcpy(units[k].ad,"ork_dovusculeri");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"mizrakcilar")!=NULL){
                    strcpy(units[k].ad,"mizrakcilar");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"varg_binicileri")!=NULL){
                    strcpy(units[k].ad,"varg_binicileri");
                    unitjson(arr,&j,&k,units);
                }
                if(strstr(arr[j],"troller")!=NULL){
                    strcpy(units[k].ad,"troller");
                    unitjson(arr,&j,&k,units);
                }
                j++;
            }while(strstr(arr[j],"insan_imparatorlugu")==NULL && j<i);
        }
        j++;
    }
}
void HeroJsonCek(char* array,hero* heroes){
    FILE* file;
    file=fopen("Files\\heroes.json","r");
    if(file==NULL){
        printf("Dosya 'Files/heroes.json' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.");
    }
    else{
        fread(array,sizeof(char),2500,file);
    }
    fclose(file);
    array[strlen(array)-1]='\0';
    HeroJsonAta(array,heroes);
}
void herojson(char arr[][90],int* tempj,int* tempk,hero* heroes) {
    int j = *tempj;
    int k = *tempk;
    j++;
    if(strstr(arr[j],"bonus_turu")!=NULL)
        sscanf(arr[j++],"bonus_turu: %s",&(heroes[k].bonusturu));
    if(strstr(arr[j],"bonus_degeri")!=NULL)
        sscanf(arr[j++],"bonus_degeri: %f",&(heroes[k].bonusdegeri));
    if(strstr(arr[j],"aciklama")!=NULL)
        sscanf(arr[j++],"aciklama: %s",&(heroes[k].etkiledigibirim));
    k++;
    *tempj = j;
    *tempk = k;
}
void HeroJsonAta(char* array,hero* heroes){
    int i=0,j=0;
    char arr[56][90];
    char* deneme=strtok(array,"\n");
    while(deneme!=NULL){
        basBoslukKaldir(deneme);
        karakterKaldir(deneme,'"');
        strcpy(arr[i++],deneme);
        deneme=strtok(NULL,"\n");
        if(deneme==NULL)
           break;
    }
    int k=0;
    while(j<i){
        if(strstr(arr[j], "insan_imparatorlugu") != NULL){
            do{
                strcpy(heroes[k].irk,"insan");
                if(strstr(arr[j],"Alparslan")!=NULL){
                    strcpy(heroes[k].ad,"Alparslan");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Mete_Han")!=NULL){
                    strcpy(heroes[k].ad,"Mete_Han");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Fatih_Sultan_Mehmet")!=NULL){
                    strcpy(heroes[k].ad,"Fatih_Sultan_Mehmet");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Yavuz_Sultan_Selim")!=NULL){
                    strcpy(heroes[k].ad,"Yavuz_Sultan_Selim");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Tugrul_Bey")!=NULL){
                    strcpy(heroes[k].ad,"Tugrul_Bey");
                    herojson(arr,&j,&k,heroes);
                }
                j++;
            }while(strstr(arr[j],"ork_legi")==NULL && j<i);
        }
        if(strstr(arr[j], "ork_legi") != NULL){
            do{
                karakterKaldir(arr[j],'"');
                strcpy(heroes[k].irk,"ork");
                if(strstr(arr[j],"Goruk_Vahsi")!=NULL){
                    strcpy(heroes[k].ad,"Goruk_Vahsi");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Thruk_Kemikkiran")!=NULL){
                    strcpy(heroes[k].ad,"Thruk_Kemikkiran");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Vrog_Kafakiran")!=NULL){
                    strcpy(heroes[k].ad,"Vrog_Kafakiran");
                    herojson(arr,&j,&k,heroes);
                }
                if(strstr(arr[j],"Ugar_Zalim")!=NULL){
                    strcpy(heroes[k].ad,"Ugar_Zalim");
                    herojson(arr,&j,&k,heroes);
                }
                j++;
            }while(strstr(arr[j],"insan_imparatorlugu")==NULL && j<i);
        }
        j++;
    }
}
void CreatureJsonCek(char* array,creature* creatures){
    FILE* file;
    file=fopen("Files\\creatures.json","r");
    if(file==NULL){
        printf("Dosya 'Files/creatures.json' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.");
    }
    else{
        fread(array,sizeof(char),2500,file);
    }
    fclose(file);
    array[strlen(array)-1]='\0';
    CreatureJsonAta(array,creatures);
}
void creaturejson(char arr[][100],int* tempj,int* tempk,creature* creatures) {
    int j = *tempj;
    int k = *tempk;
    j++;
    if(strstr(arr[j],"etki_degeri")!=NULL)
        sscanf(arr[j++],"etki_degeri: %f",&(creatures[k].etkidegeri));
    if(strstr(arr[j],"etki_turu")!=NULL)
        sscanf(arr[j++],"etki_turu: %s",&(creatures[k].etkituru));
    if(strstr(arr[j],"aciklama")!=NULL)
        sscanf(arr[j++],"aciklama: %s",&(creatures[k].etkiledigibirim));
    k++;
    *tempj = j;
    *tempk = k;
}
void CreatureJsonAta(char* array,creature* creatures){
    int i=0,j=0;
    char arr[70][100];
    char* deneme=strtok(array,"\n");
    while(deneme!=NULL){
        basBoslukKaldir(deneme);
        karakterKaldir(deneme,'"');
        strcpy(arr[i++],deneme);
        deneme=strtok(NULL,"\n");
        if(deneme==NULL)
           break;
    }
    int k=0;
    while(j<i){
        if(strstr(arr[j],"insan_imparatorlugu") != NULL){
            do{
                strcpy(creatures[k].irk,"insan_canavari");
                if(strstr(arr[j],"Ejderha")!=NULL){
                    strcpy(creatures[k].ad,"Ejderha");
                    creaturejson(arr,&j,&k,creatures);
                }
                if(strstr(arr[j],"Agri_Dagi_Devleri")!=NULL){
                    strcpy(creatures[k].ad,"Agri_Dagi_Devleri");
                    creaturejson(arr,&j,&k,creatures);
                }
                if(strstr(arr[j],"Tepegoz")!=NULL){
                    strcpy(creatures[k].ad,"Tepegoz");
                    creaturejson(arr,&j,&k,creatures);
                }
                if(strstr(arr[j],"Karakurt")!=NULL){
                    strcpy(creatures[k].ad,"Karakurt");
                    creaturejson(arr,&j,&k,creatures);
                }
                if(strstr(arr[j],"Samur")!=NULL){
                    strcpy(creatures[k].ad,"Samur");
                    creaturejson(arr,&j,&k,creatures);
                }
                j++;
            }while(strstr(arr[j],"ork_legi")==NULL && j<i);
        }
        if(strstr(arr[j], "ork_legi") != NULL){
            do{
            strcpy(creatures[k].irk,"ork_canavari");
            if(strstr(arr[j],"Kara_Troll")!=NULL){
                strcpy(creatures[k].ad,"Kara_Troll");
                creaturejson(arr,&j,&k,creatures);
            }
            if(strstr(arr[j],"Golge_Kurtlari")!=NULL){
                strcpy(creatures[k].ad,"Golge_Kurtlari");
                creaturejson(arr,&j,&k,creatures);
            }
            if(strstr(arr[j],"Camur_Devleri")!=NULL){
                strcpy(creatures[k].ad,"Camur_Devleri");
                creaturejson(arr,&j,&k,creatures);
            }
            if(strstr(arr[j],"Ates_Iblisi")!=NULL){
                strcpy(creatures[k].ad,"Ates_Iblisi");
                creaturejson(arr,&j,&k,creatures);
            }
            if(strstr(arr[j],"Makrog_Savas_Beyi")!=NULL) {
                strcpy(creatures[k].ad,"Makrog_Savas_Beyi");
                creaturejson(arr,&j,&k,creatures);
            }
            if(strstr(arr[j],"Buz_Devleri")!=NULL){
                strcpy(creatures[k].ad,"Buz_Devleri");
                creaturejson(arr,&j,&k,creatures);
            }
            j++;
            }while(strstr(arr[j],"insan_imparatorlugu")==NULL && j<i);
        }
        j++;
    }
}
void ResearchJsonCek(char* array,research* researches){
    FILE* file;
    file=fopen("Files\\research.json","r");
    if(file==NULL){
        printf("Dosya 'Files/research.json' bulunamadi. Lutfen dosyanin dogru isimde ve dogru dizinde oldugundan emin olun.");
    }
    else{
        fread(array,sizeof(char),2500,file);
    }
    fclose(file);
    array[strlen(array)-1]='\0';
    ResearchJsonAta(array,researches);
}
void researchjson(char arr[][90],int* tempj,int* tempk,int* templ,research* researches) {
    int j = *tempj;
    int k = *tempk;
    int l = *templ;
    if(strstr(arr[j++],"seviye_1")!=NULL){
        if(strstr(arr[j],"deger"))
            sscanf(arr[j++],"deger: %f",&(researches[k].degerler[l++]));
        if(strstr(arr[j],"aciklama")!=NULL)
            sscanf(arr[j++],"aciklama: %s",&(researches[k].etkiledigibirim));
        j++;
    }
    if(strstr(arr[j++],"seviye_2")!=NULL){
        if(strstr(arr[j],"deger"))
            sscanf(arr[j++],"deger: %f",&(researches[k].degerler[l++]));
        j+=2;
    }
    if(strstr(arr[j++],"seviye_3")!=NULL){
        if(strstr(arr[j],"deger"))
            sscanf(arr[j++],"deger: %f",&(researches[k].degerler[l++]));
        j+=3;
    }
    k++;
    l=0;
    *tempj = j;
    *tempk = k;
    *templ = l;
}
void ResearchJsonAta(char* array,research* researches){
    int i=0,j=1;
    char arr[60][90];
    char* deneme=strtok(array,"\n");
    while(deneme!=NULL){
        basBoslukKaldir(deneme);
        karakterKaldir(deneme,'"');
        strcpy(arr[i++],deneme);
        deneme=strtok(NULL,"\n");
        if(deneme==NULL)
           break;
    }
    int l=0,k=0;
    while(j<i){
        if(strstr(arr[j++],"savunma_ustaligi")!=NULL){
            strcpy(researches[k].ad,"savunma_ustaligi");
            strcpy(researches[k].bonusturu,"savunma");
            researchjson(arr,&j,&k,&l,researches);
        }
        if(strstr(arr[j++],"saldiri_gelistirmesi")!=NULL){
            strcpy(researches[k].ad,"saldiri_gelistirmesi");
            strcpy(researches[k].bonusturu,"saldiri");
            researchjson(arr,&j,&k,&l,researches);
        }
        if(strstr(arr[j++],"elit_egitim")!=NULL){
            strcpy(researches[k].ad,"elit_egitim");
            strcpy(researches[k].bonusturu,"kritik_sans");
            researchjson(arr,&j,&k,&l,researches);
        }
        if(strstr(arr[j++],"kusatma_ustaligi")!=NULL){
            strcpy(researches[k].ad,"kusatma_ustaligi");
            strcpy(researches[k].bonusturu,"saldiri");
            researchjson(arr,&j,&k,&l,researches);
        }
        j++;
    }
}
// CURL'den gelen verileri saklamak için callback fonksiyonu
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("Not enough memory (realloc returned NULL)\n");
        return 0; // out of memory!
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// JSON verisini almak için fonksiyon
char* fetch_json_data(const char* url) {
    struct MemoryStruct chunk;

    chunk.memory = malloc(1); // Başlangıçta bellek ayır
    chunk.size = 0; // Veri boyutu başlangıçta 0

    curl_global_init(CURL_GLOBAL_DEFAULT); // CURL'i başlat

    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // İsteği gönder
        CURLcode res = curl_easy_perform(curl);

        // Hata kontrolü
        if (res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            free(chunk.memory); // Belleği serbest bırak
            curl_easy_cleanup(curl); // CURL'i temizle
            curl_global_cleanup(); // CURL'i temizle
            return NULL;
        }

        // CURL temizle
        curl_easy_cleanup(curl);
    }

    curl_global_cleanup(); // CURL'i temizle
    return chunk.memory; // Gelen veriyi döndür
}

void SenaryoJsonCek(char* array,human* insan,ork_* ork,unit* units,hero* heroes,creature* creatures,research* researches){
    int senaryoid;
    char* url;
    printf("Hangi senaryoyu secmek istersiniz(1-10): ");
    scanf("%d",&senaryoid);
    switch(senaryoid){
        case 1:
              url="https://yapbenzet.org.tr/1.json";
            break;
        case 2:
            url="https://yapbenzet.org.tr/2.json";
            break;
        case 3:
            url="https://yapbenzet.org.tr/3.json";
            break;
        case 4:
            url="https://yapbenzet.org.tr/4.json";
            break;
        case 5:
            url="https://yapbenzet.org.tr/5.json";
            break;
        case 6:
            url="https://yapbenzet.org.tr/6.json";
            break;
        case 7:
            url="https://yapbenzet.org.tr/7.json";
            break;
        case 8:
            url="https://yapbenzet.org.tr/8.json";
            break;
        case 9:
            url="https://yapbenzet.org.tr/9.json";
            break;
        case 10:
            url="https://yapbenzet.org.tr/10.json";
            break;
        default:
            break;
    }
    array=fetch_json_data(url);
    array[strlen(array)-1]='\0';
    SenaryoJsonAta(array,insan,ork,units,heroes,creatures,researches);
}
void SenaryoJsonAta(char* array,human* insan,ork_* ork,unit* units,hero* heroes,creature* creatures,research* researches){
    int i=0,j=0;
    char arr[30][60];
    char* deneme=strtok(array,"\n");
    while(deneme!=NULL){
        basBoslukKaldir(deneme);
        karakterKaldir(deneme,'"');
        strcpy(arr[i++],deneme);
        if(deneme==NULL)
           break;
        deneme=strtok(NULL,"\n");
    }
    int k1=0,k2=0,k3=0,k4=0;
    while(j<i){
        if(strstr(arr[j], "insan_imparatorlugu") != NULL){
          do{
                if(strstr(arr[j],"birimler")!=NULL){
                    do{
                        for(int x=0;x<8;x++){
                            if(strstr(arr[j],units[x].ad)!=NULL){
                                char* ptr=strchr(arr[j],':');
                                insan->birimsayisi[k1]=atoi(ptr+1);
                                strcpy(insan->birimler[k1++],units[x].ad);
                            }
                        }
                        j++;
                    }while(strstr(arr[j],"}")==NULL && strstr(arr[j],"},")==NULL);
                }
                if(strstr(arr[j],"kahramanlar")!=NULL){
                    karakterKaldir(arr[j],'[');
                    karakterKaldir(arr[j],']');
                    for(int x=0;x<9;x++){
                        if(strstr(arr[j],heroes[x].ad)!=NULL){
                            char* ptr=strchr(arr[j],':');
                            if(strstr(arr[j],",")!=NULL){
                                char* ptr2=strchr(arr[j],',');
                                strncpy(insan->kahramanlar[k2++],ptr+1,ptr2-ptr-1);
                                strcpy(insan->kahramanlar[k2++],ptr2+1);
                            }
                            else{
                                 strcpy(insan->kahramanlar[k2++],ptr+1);
                            }
                        }
                    }
                }
                if(strstr(arr[j],"canavarlar")!=NULL){
                    karakterKaldir(arr[j],'[');
                    karakterKaldir(arr[j],']');
                    for(int x=0;x<11;x++){
                        if(strstr(arr[j],creatures[x].ad)!=NULL){
                            char* ptr=strchr(arr[j],':');
                            strcpy(insan->canavarlar[k3++],ptr+1);
                        }
                    }
                }
                if(strstr(arr[j],"arastirma_seviyesi")!=NULL){
                    j++;
                    for(int x=0;x<4;x++){
                        if(strstr(arr[j],researches[x].ad)!=NULL){
                            char* ptr=strchr(arr[j],':');
                            insan->arastirmaseviyesi=atoi(ptr+1);
                            strcpy(insan->arastirmalar[k4++],arr[j++]);
                        }
                    }
                }
                j++;
          }while(strstr(arr[j],"ork_legi")==NULL && j<i);
          k1=0;
          k2=0;
          k3=0;
          k4=0;
        }
        if(strstr(arr[j], "ork_legi") != NULL){
          do{
            if(strstr(arr[j],"birimler")!=NULL){
                do{
                    for(int x=0;x<8;x++){
                        if(strstr(arr[j],units[x].ad)!=NULL){
                            char* ptr=strchr(arr[j],':');
                            ork->birimsayisi[k1]=atoi(ptr+1);
                            strcpy(ork->birimler[k1++],units[x].ad);
                        }
                    }
                    j++;
                }while(strstr(arr[j],"}")==NULL && strstr(arr[j],"},")==NULL);
            }
            if(strstr(arr[j],"kahramanlar")!=NULL){
                karakterKaldir(arr[j],'[');
                karakterKaldir(arr[j],']');
                for(int x=0;x<9;x++){
                    if(strstr(arr[j],heroes[x].ad)!=NULL){
                        char* ptr=strchr(arr[j],':');
                        if(strstr(arr[j],",")!=NULL){
                            char* ptr2=strchr(arr[j],',');
                            strncpy(ork->kahramanlar[k2++],ptr+1,ptr2-ptr-1);
                            strcpy(ork->kahramanlar[k2++],ptr2+1);
                        }
                        else{
                            strcpy(ork->kahramanlar[k2++],ptr+1);
                        }
                    }
                }
            }
            if(strstr(arr[j],"canavarlar")!=NULL){
                karakterKaldir(arr[j],'[');
                karakterKaldir(arr[j],']');
                for(int x=0;x<11;x++){
                    if(strstr(arr[j],creatures[x].ad)!=NULL){
                        char* ptr=strchr(arr[j],':');
                        strcpy(ork->canavarlar[k3++],ptr+1);
                    }
                }
            }
            if(strstr(arr[j],"arastirma_seviyesi")!=NULL){
                j++;
                for(int x=0;x<4;x++){
                    if(strstr(arr[j],researches[x].ad)!=NULL){
                        char* ptr=strchr(arr[j],':');
                        ork->arastirmaseviyesi=atoi(ptr+1);
                        strcpy(ork->arastirmalar[k4++],arr[j++]);
                    }
                }
            }
            j++;
          }while(strstr(arr[j],"insan_imparatorlugu")==NULL && j<i);
          j--;
          k1=0;
          k2=0;
          k3=0;
          k4=0;
        }
        j++;
    }
}
void StatlariHesapla(unit* units,human* insan,ork_* ork,int adim){
    insan->toplamsaldiri=0;
    insan->toplamsavunma=0;
    ork->toplamsaldiri=0;
    ork->toplamsavunma=0;
    for(int j=0;j<4 && strlen(insan->birimler[j])!=0 && strlen(ork->birimler[j])!=0;j++){
      for(int i=0;i<8;i++){
            if(strstr(units[i].ad,insan->birimler[j])!=NULL && units[i].saglik!=0){
                float saldirigucu = KritikVurusHesapla(units[i].saldiri,units[i].kritiksans,adim);
                insan->toplamsaldiri += saldirigucu * insan->birimsayisi[j];
                insan->toplamsavunma += units[i].savunma * insan->birimsayisi[j];
            }
            if(strstr(units[i].ad,ork->birimler[j])!=NULL && units[i].saglik!=0){
                float saldirigucu = KritikVurusHesapla(units[i].saldiri,units[i].kritiksans,adim);
                ork->toplamsaldiri += saldirigucu * ork->birimsayisi[j];
                ork->toplamsavunma += units[i].savunma * ork->birimsayisi[j];
            }
        }
    }
}
void KahramanEtkisiniEkle(unit* units,human* insan,ork_* ork,hero* heroes){
    for(int j=0;j<4 && strlen(insan->birimler[j])!=0 ;j++){
       for(int k=0;k<2 && strlen(insan->kahramanlar[k]);k++){
            for(int i=0;i<9;i++){
                if(strstr(insan->kahramanlar[k],heroes[i].ad)!=NULL){
                    if(strstr(heroes[i].etkiledigibirim,insan->birimler[j])!=NULL){
                        for(int x=0;x<8;x++){
                            if(strstr(heroes[i].etkiledigibirim,units[x].ad)!=NULL){
                                if(strstr(heroes[i].bonusturu,"saldiri")!=NULL)
                                    units[x].saldiri *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"savunma")!=NULL)
                                    units[x].savunma *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"kritik_sans")!=NULL)
                                    units[x].kritiksans *= ((heroes[i].bonusdegeri/100)+1);
                            }
                        }
                    }
                    else if(strstr(heroes[i].etkiledigibirim,"tum_birimler")!=NULL){
                        for(int x=0;x<8;x++){
                            if(strstr(heroes[i].etkiledigibirim,units[x].ad)!=NULL){
                                if(strstr(heroes[i].bonusturu,"saldiri")!=NULL)
                                    units[x].saldiri *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"savunma")!=NULL)
                                    units[x].savunma *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"kritik_sans")!=NULL)
                                    units[x].kritiksans *= ((heroes[i].bonusdegeri/100)+1);
                            }
                        }
                    }
                }
                if(strstr(ork->kahramanlar[k],heroes[i].ad)!=NULL){
                    if(strstr(heroes[i].etkiledigibirim,ork->birimler[j])!=NULL){
                        for(int x=0;x<8;x++){
                            if(strstr(heroes[i].etkiledigibirim,units[x].ad)!=NULL){
                                if(strstr(heroes[i].bonusturu,"saldiri")!=NULL)
                                    units[x].saldiri *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"savunma")!=NULL)
                                    units[x].savunma *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"kritik_sans")!=NULL)
                                    units[x].kritiksans *= ((heroes[i].bonusdegeri/100)+1);
                            }
                        }
                    }
                    else if(strstr(heroes[i].etkiledigibirim,"tum_birimler")!=NULL){
                        for(int x=0;x<8;x++){
                            if(strstr(units[x].irk,"ork")!=NULL){
                                if(strstr(heroes[i].bonusturu,"saldiri")!=NULL)
                                    units[x].saldiri *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"savunma")!=NULL)
                                    units[x].savunma *= ((heroes[i].bonusdegeri/100)+1);
                                if(strstr(heroes[i].bonusturu,"kritik_sans")!=NULL)
                                    units[x].kritiksans *= ((heroes[i].bonusdegeri/100)+1);
                            }
                        }
                    }
                }
            }
        }
    }
}
void CanavarEtkisiniEkle(unit* units,human* insan,ork_* ork,creature* creatures){
    for(int j=0;j<4 && strlen(insan->birimler[j])!=0 && strlen(ork->birimler[j])!=0;j++){
       for(int i=0;i<11;i++){
            if(strstr(insan->canavarlar[0],creatures[i].ad)!=NULL){
                if(strstr(creatures[i].etkiledigibirim,insan->birimler[j])!=NULL){
                    for(int x=0;x<8;x++){
                        if(strstr(creatures[i].etkiledigibirim,units[x].ad)!=NULL){
                            if(strstr(creatures[i].etkituru,"saldiri")!=NULL)
                                units[x].saldiri *= ((creatures[i].etkidegeri/100)+1);
                            if(strstr(creatures[i].etkituru,"savunma")!=NULL)
                                units[x].savunma *= ((creatures[i].etkidegeri/100)+1);
                            if(strstr(creatures[i].etkituru,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((creatures[i].etkidegeri/100)+1);
                        }
                    }
                }
            }
            if(strstr(ork->canavarlar[0],creatures[i].ad)!=NULL){
                if(strstr(creatures[i].etkiledigibirim,ork->birimler[j])!=NULL){
                    for(int x=0;x<8;x++){
                        if(strstr(creatures[i].etkiledigibirim,units[x].ad)!=NULL){
                            if(strstr(creatures[i].etkituru,"saldiri")!=NULL)
                                units[x].saldiri *= ((creatures[i].etkidegeri/100)+1);
                            if(strstr(creatures[i].etkituru,"savunma")!=NULL)
                                units[x].savunma *= ((creatures[i].etkidegeri/100)+1);
                            if(strstr(creatures[i].etkituru,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((creatures[i].etkidegeri/100)+1);
                        }
                    }
                }
            }
        }
    }
}
void ArastirmaEtkisiniEkle(unit* units,human* insan,ork_* ork,research* researches){
    int kontrol1=0,kontrol2=0;
    for(int j=0;j<4 && strlen(insan->birimler[j])!=0 && strlen(ork->birimler[j])!=0;j++){
       for(int i=0;i<4;i++){
            if(strstr(insan->arastirmalar[0],researches[i].ad)!=NULL){
                if(strstr(researches[i].etkiledigibirim,insan->birimler[j])!=NULL){
                    for(int x=0;x<8;x++){
                        if(strstr(researches[i].etkiledigibirim,units[x].ad)!=NULL){
                            if(strstr(researches[i].bonusturu,"saldiri")!=NULL)
                                units[x].saldiri *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"savunma")!=NULL)
                                units[x].savunma *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                        }
                    }
                }
                else if(strstr(researches[i].etkiledigibirim,"tum_birimler")!=NULL && kontrol1!=1){
                    for(int x=0;x<8;x++){
                        if(strstr(units[x].irk,"insan")!=NULL){
                            if(strstr(researches[i].bonusturu,"saldiri")!=NULL)
                                units[x].saldiri *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"savunma")!=NULL)
                                units[x].savunma *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((researches[i].degerler[insan->arastirmaseviyesi-1]/100)+1);
                            kontrol1=1;
                        }
                    }
                }
            }
            if(strstr(ork->arastirmalar[0],researches[i].ad)!=NULL){
                if(strstr(researches[i].etkiledigibirim,ork->birimler[j])!=NULL){
                    for(int x=0;x<8;x++){
                        if(strstr(researches[i].etkiledigibirim,units[x].ad)!=NULL){
                            if(strstr(researches[i].bonusturu,"saldiri")!=NULL)
                                units[x].saldiri *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"savunma")!=NULL)
                                units[x].savunma *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                        }
                    }
                }
                else if(strstr(researches[i].etkiledigibirim,"tum_birimler")!=NULL && kontrol2!=1){
                    for(int x=0;x<8;x++){
                        if(strstr(units[x].irk,"ork")!=NULL){
                            if(strstr(researches[i].bonusturu,"saldiri")!=NULL)
                                units[x].saldiri *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"savunma")!=NULL)
                                units[x].savunma *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                            if(strstr(researches[i].bonusturu,"kritik_sans")!=NULL)
                                units[x].kritiksans *= ((researches[i].degerler[ork->arastirmaseviyesi-1]/100)+1);
                            kontrol2=1;
                        }
                    }
                }
            }
        }
    }
}
void YorgunlukEtkisiniEkle(unit* units,human* insan,ork_* ork){
    int j=0;
    do{
       for(int i=0;i<8;i++){
            if(strstr(insan->birimler[j],units[i].ad)!=NULL){
                units[i].saldiri *= 0.9;
                units[i].savunma *= 0.9;
            }
            if(strstr(ork->birimler[j],units[i].ad)!=NULL){
                units[i].saldiri *= 0.9;
                units[i].savunma *= 0.9;
            }
        }
        j++;
    }while(j<4 && strlen(insan->birimler[j])!=0);
}
int KritikVurusHesapla(int unitsaldiri,float unitkritiksans,int adim){
        if(unitkritiksans>0){
            int kritikvurussayisi = (100/unitkritiksans);
            if(adim%kritikvurussayisi==0)
                return 1.5*unitsaldiri;
            else
                return unitsaldiri;
        }
        else
            return unitsaldiri;
}
float NetHasarHesapla(float toplamsaldiri,float toplamsavunma){
    float nethasar = toplamsaldiri*(1-(toplamsavunma/toplamsaldiri));
    if(nethasar<0)
        return 0;
    else
        return nethasar;
}
void OrkOrantiliHasarDagilimi(unit* units,ork_* ork,int nethasar,float toplamsavunma){
    for(int j=0;j<4 && strlen(ork->birimler[j])!=0 ;j++){
        for(int i=0;i<8;i++){
            if(strstr(units[i].ad,ork->birimler[j])!=NULL && units[i].saglik!=0){
                float hasar = nethasar*((units[i].savunma*ork->birimsayisi[j])/toplamsavunma);
                int azalansaglik = ((hasar)/(ork->birimsayisi[j]));
                if(units[i].saglik<azalansaglik)
                    units[i].saglik = 0;
                else
                    units[i].saglik -= azalansaglik;
                if(units[i].saglik==0)
                    ork->birimsayisi[j]=0;
            }
        }
    }
}
void insanOrantiliHasarDagilimi(unit* units,human* insan,int nethasar,float toplamsavunma){
    for(int j=0;j<4 && strlen(insan->birimler[j])!=0 ;j++){
        for(int i=0;i<8;i++){
            if(strstr(units[i].ad,insan->birimler[j])!=NULL && units[i].saglik!=0){
                float hasar = nethasar*((units[i].savunma*insan->birimsayisi[j])/toplamsavunma);
                int azalansaglik = ((hasar)/(insan->birimsayisi[j]));
                if(units[i].saglik<azalansaglik)
                    units[i].saglik = 0;
                else
                    units[i].saglik -= azalansaglik;
                if(units[i].saglik==0)
                    insan->birimsayisi[j]=0;
            }
        }
    }
}
void AdimlariMetneYazmak(unit* units,human* insan,ork_* ork,int adim,float toplamsaldiri,float toplamsavunma,float nethasar){
    FILE* file;
    if(adim==0){
        file=fopen("savas_sim.txt","w");
        fclose(file);
    }
    file=fopen("savas_sim.txt","a");
    if(file==NULL)
        printf("Dosya acilamadi\n");
    if(adim%2==1){
        fprintf(file,"%d.adim:\n\tInsanlar\n",adim);
        for(int j=0;j<4 && strlen(insan->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].irk,"insan")!=NULL){
                    if(strstr(units[i].ad,insan->birimler[j])!=NULL){
                       fprintf(file,"\t\t%s saldiri gucu = %d * %d = %d\n",insan->birimler[j],insan->birimsayisi[j],units[i].saldiri,units[i].saldiri*insan->birimsayisi[j]);
                    }
                }
            }
        }
        fprintf(file,"\tOrklar\n",adim);
        for(int j=0;j<4 && strlen(ork->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].irk,"ork")!=NULL){
                    if(strstr(units[i].ad,ork->birimler[j])!=NULL){
                       fprintf(file,"\t\t%s savunma gucu = %d * %d = %d\n",ork->birimler[j],ork->birimsayisi[j],units[i].savunma,units[i].savunma*ork->birimsayisi[j]);
                    }
                }
            }
        }
        fprintf(file,"\tInsan toplam saldiri gucu: %.2f\n\tOrk savunma gucu: %.2f\n\tNet hasar: %.2f\n",toplamsaldiri,toplamsavunma,nethasar);
        for(int j=0;j<4 && strlen(insan->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].ad,insan->birimler[j])!=NULL)
                    fprintf(file,"\tKalan %s : %d -- Kalan saglik:%d\n",insan->birimler[j],insan->birimsayisi[j],units[i].saglik);
            }
        }
        for(int j=0;j<4 && strlen(ork->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].ad,ork->birimler[j])!=NULL)
                   fprintf(file,"\tKalan %s : %d -- Kalan saglik:%d\n",ork->birimler[j],ork->birimsayisi[j],units[i].saglik);
            }
        }
    }
    else{
        fprintf(file,"%d.adim:\n\tOrklar\n",adim);
        for(int j=0;j<4 && strlen(ork->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].irk,"ork")!=NULL){
                    if(strstr(units[i].ad,ork->birimler[j])!=NULL){
                       fprintf(file,"\t\t%s saldiri gucu = %d * %d = %d\n",ork->birimler[j],ork->birimsayisi[j],units[i].saldiri,units[i].saldiri*ork->birimsayisi[j]);
                    }
                }
            }
        }
        fprintf(file,"\tInsanlar\n",adim);
        for(int j=0;j<4 && strlen(insan->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].irk,"insan")!=NULL){
                    if(strstr(units[i].ad,insan->birimler[j])!=NULL){
                       fprintf(file,"\t\t%s savunma gucu = %d * %d = %d\n",insan->birimler[j],insan->birimsayisi[j],units[i].savunma,units[i].savunma*insan->birimsayisi[j]);
                    }
                }
            }
        }
        fprintf(file,"\tOrk toplam saldiri gucu: %.2f\n\tInsan savunma gucu: %.2f\n\tNet hasar: %.2f\n",toplamsaldiri,toplamsavunma,nethasar);
        for(int j=0;j<4 && strlen(insan->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].ad,insan->birimler[j])!=NULL)
                    fprintf(file,"\tKalan %s : %d -- Kalan saglik:%d\n",insan->birimler[j],insan->birimsayisi[j],units[i].saglik);
            }
        }
        for(int j=0;j<4 && strlen(ork->birimler[j])!=0;j++){
            for(int i=0;i<8;i++){
                if(strstr(units[i].ad,ork->birimler[j])!=NULL)
                   fprintf(file,"\tKalan %s : %d -- Kalan saglik:%d\n",ork->birimler[j],ork->birimsayisi[j],units[i].saglik);
            }
        }
    }
    if(insan->birimsayisi[0] == 0 &&  insan->birimsayisi[1] == 0 && insan->birimsayisi[2] == 0 && insan->birimsayisi[3] == 0)
        fprintf(file,"Insanlar oldu.Orklar kazandi");
    else if(ork->birimsayisi[0] == 0 &&  ork->birimsayisi[1] == 0 && ork->birimsayisi[2] == 0 && ork->birimsayisi[3] == 0)
        fprintf(file,"Orklar oldu.Insanlar kazandi");
    fclose(file);
}
void GrafikCagir(unit* units,hero* heroes,creature* creatures,human* insan,ork_* ork) {
    int okcu=0,suvari=0,piyade=0,kusatma=0,trol=0,varg=0,ork_dovuscusu=0,mizrakli=0,insanhero=0,insancanavar=0,orkhero=0,orkcanavar=0,okcu_s=0,suvari_s=0,piyade_s=0,kusatma_s=0,trol_s=0,varg_s=0,ork_dovuscusu_s=0,mizrakli_s=0;
    for(int i=0;i<11;i++) {
        if(strstr(insan->canavarlar[0],creatures[i].ad)!=NULL)
            insancanavar=1;
        if(strstr(ork->canavarlar[0],creatures[i].ad)!=NULL)
            orkcanavar=1;
    }
    for(int j=0;j<2;j++) {
        for(int i=0;i<9;i++) {
            if(strstr(insan->kahramanlar[j],heroes[i].ad)!=NULL)
                insanhero++;
            if(strstr(ork->kahramanlar[j],heroes[i].ad)!=NULL)
                orkhero++;
        }
    }
    for(int j=0;j<4;j++){
        for(int i=0;i<8;i++){
            if(strstr(insan->birimler[j],units[i].ad)!=NULL) {
                if(strstr(units[i].ad,"piyade")!=NULL) {
                    piyade=insan->birimsayisi[j];
                    piyade_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"okcu")!=NULL) {
                    okcu=insan->birimsayisi[j];
                    okcu_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"suvari")!=NULL) {
                    suvari=insan->birimsayisi[j];
                    suvari_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"kusatma_makine")!=NULL) {
                    kusatma=insan->birimsayisi[j];
                    kusatma_s=units[i].saglik;
                }
            }
            if(strstr(ork->birimler[j],units[i].ad)!=NULL) {
                if(strstr(units[i].ad,"ork_dovuscu")!=NULL) {
                    ork_dovuscusu=ork->birimsayisi[j];
                    ork_dovuscusu_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"mizrakci")!=NULL) {
                    mizrakli=ork->birimsayisi[j];
                    mizrakli_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"varg_binici")!=NULL) {
                    varg=ork->birimsayisi[j];
                    varg_s=units[i].saglik;
                }
                else if(strstr(units[i].ad,"trol")!=NULL) {
                    trol=ork->birimsayisi[j];
                    trol_s=units[i].saglik;
                }
            }
        }
    }
    int initialValues[20]={okcu,kusatma,suvari,piyade,ork_dovuscusu,varg,trol,mizrakli,insanhero,insancanavar,orkhero,orkcanavar,okcu_s,kusatma_s,suvari_s,piyade_s,ork_dovuscusu_s,varg_s,trol_s,mizrakli_s};
    DrawGraph(initialValues);
}
void karakterKaldir(char* str, char karakter){
    int i, j = 0;
    int uzunluk = strlen(str);
    for (i = 0; i < uzunluk; i++) {// Ayný string üzerinde iþlem yapýyoruz
        if (str[i] != karakter)
            str[j++] = str[i];  // Karakter farklýysa diziyi güncelle
    }
    str[j] = '\0';  // Yeni sonlandýrma karakteri ekle
}
void basBoslukKaldir(char *str) {
    int i = 0, j = 0;
    while (isspace(str[i]))// Ýlk boþluk olmayan karakteri bul
        i++;
    while (str[i] != '\0') // Stringin geri kalanýný baþa kopyala
        str[j++] = str[i++];
    str[j] = '\0'; // Kalan kýsmý sonlandýr
}
