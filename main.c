#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Oyuncuya ait ana struct 
typedef struct {
    int can;
    int guc;
    int envanter_boyutu;
    char **envanter; 
} Oyuncu;
// Odalara ait ana struct
typedef struct {
    char *aciklama;
    char *esya;
    int yaratik_var;
    int yaratik_can;
} Oda;
//Main fonksiyoon
int main() {
	srand(time(NULL));
	
    Oyuncu oyuncu;
    int oda_sayisi=6;
    int canavar_atak_gucu;
    Oda *odalar = (Oda *)malloc(oda_sayisi * sizeof(Oda)); 
    if (odalar == NULL) {
        printf("Memory allocation error!\n");
        return 1;
    }
    int mevcut_oda; 
    char komut[50];
    oyuncu_baslangic(&oyuncu);
    odalar_baslangic(odalar, oda_sayisi);
    mevcut_oda = rastgele_oda_sec(oda_sayisi);
    //yapýilabilecek komutlari yazdiran kod blogu
    printf("Zindana hos geldiniz! 'oda_bak', 'git', 'envanter', 'al', 'saldir', 'kaydet', 'yukle', 'cikis' komutlarini kullanin.\n\n");
	
	printf("- oda_bak    : Bulundugunuz odanin etrafina bakarak detayli bilgi alabilirsiniz.\n");
	printf("- git<yon>   : Belirttiginiz yone dogru ilerlemek icin kullanilir. (ornegin: 'git 2')= oda 2 ye gitme anlamýna gelir\n");
	printf("- envanter   : Uzerinizde bulunan esyalari ve malzemeleri goruntuleyin.\n");
	printf("- al<nesne>  : Odada bulunan bir nesneyi almak icin kullanilir. (ornegin: 'al kýlýç')\n");
	printf("- saldir     : Dusmana saldirmak icin kullanilir. (Once dusmanin varligini kontrol edin!)\n");
	printf("- kaydet     : Oyundaki ilerlemenizi kaydeder.\n");
	printf("- yukle      : Daha once kaydedilmis bir oyunu yukler ve kaldiginiz yerden devam etmenizi saglar.\n");
	printf("- cikis      : Oyundan cikmak icin kullanilir.\n");
	displayMap();
    //Girilen komutlara gore gerceklesececk islemler
    while (1) {
        printf("\n> ");
        fgets(komut, sizeof(komut), stdin);
        komut[strcspn(komut, "\n")] = '\0';
        
        if (strncmp(komut, "oda_bak", 7) == 0) {
            oda_bak(&odalar[mevcut_oda]);
            
        } else if (strncmp(komut, "git", 3) == 0) {
            int yon = atoi(&komut[4]);
            if (yon >= 0 && yon < oda_sayisi) {
                odaya_git(&mevcut_oda, yon, oda_sayisi,odalar);
        }else {
                printf("Gecersiz oda numarasi!\n");
            }
		}
		else if (strncmp(komut, "envanter", 8) == 0) {
            envanter_goster(&oyuncu);
        }
		 else if (strncmp(komut, "al", 2) == 0) {
            esya_al(&oyuncu, &odalar[mevcut_oda]);
        }
		 else if (strncmp(komut, "saldir", 6) == 0) {
    		yaratiga_saldir(&oyuncu, &odalar[mevcut_oda], &mevcut_oda, oda_sayisi);
		}
		 else if (strncmp(komut, "kaydet", 6) == 0) {
            oyun_kaydet(&oyuncu, odalar, mevcut_oda);
        }
		 else if (strncmp(komut, "yukle", 5) == 0) {
            oyun_yukle(&oyuncu, odalar, &mevcut_oda);
        }
		 else if (strncmp(komut, "cikis", 5) == 0) {
            printf("Oyundan cikiliyor...\n");
            break;
        } 
		else {
            printf("Bilinmeyen komut!\n");
        }
        
        if (tum_yaratiklar_olduruldu_mu(odalar, oda_sayisi)) {
        	printf("\nTEBRÝKLER! Tum yaratiklari oldurdunuz. Oyunu bitirdiniz!\n");
        	break; 
    	}
        
        if (odalar[mevcut_oda].yaratik_var) {
        	int canavar_atak_gucu = (rand() % 10) + 5;
            oyuncu.can -= canavar_atak_gucu;
            if(oyuncu.can>0){
            	printf("Yaratik size saldirdi! Caniniz: %d\n", oyuncu.can);
			}
            
            if (oyuncu.can <= 0) {
                printf("Oldunuz! Rastgele bir odaya yeniden doguyorsunuz...\n");
                oyuncu_baslangic(&oyuncu);
                mevcut_oda = rastgele_oda_sec(oda_sayisi);
            }
    	}
    }
    // Odalari serbest birakan kod blogu
	odalar_serbest_birak(odalar, oda_sayisi);
	free(odalar);
	free(oyuncu.envanter);
	return 0;
}
int rastgele_oda_sec(int oda_sayisi){
	return rand() % oda_sayisi;
}
//Oyunun ana yapisini gosteren harita
void displayMap() {
    printf("\n");
    printf("                 |ARMORY(ODA-1)|                     \n");
    printf("              ____//___________\\____                   \n");
    printf("             //                    \\                    \n");
    printf("            //                      \\                      \n ");
    printf("   |LIBRARY(ODA-3)|__________|TREASURE ROOM(ODA-2)|            \n");
    printf("             \\                    //                    \n");
    printf("              \\__________________//              \n");
    printf("              |HALL OF BONES(ODA-4)|                  \n");
    printf("                    //          \\                              \n ");
    printf("               ____//____________\\____                      \n");
    printf("              //                     \\                        \n");
    printf("              \\                     //                             \n ");
    printf("    |DARK CAVE(ODA-5)|________|DUNGEON ENTRANCE(ODA-6)|        \n");
    printf("               \\                   //                        \n");
    printf("                \\_________________//                          \n");
}
// Oyuncu baslatma fonksiyonu
void oyuncu_baslangic(Oyuncu *oyuncu) {
    oyuncu->can = 100;
    oyuncu->guc = 30;
    oyuncu->envanter_boyutu = 0;
    oyuncu->envanter = (char **)malloc(5 * sizeof(char *)); 
}
// Odalari baslatma fonksiyonu
void odalar_baslangic(Oda *odalar, int oda_sayisi) {
    int i=0;
    for (i = 0; i < oda_sayisi; i++) {
        odalar[i].aciklama = (char *)malloc(100 * sizeof(char));
        sprintf(odalar[i].aciklama, "Oda %d: Kapkaranlik bir yer", i + 1);
        odalar[i].yaratik_var= rand() %2;
        if(odalar[i].yaratik_var){
        	odalar[i].yaratik_can =(rand()%70)+31;
		}
		else{
			odalar[i].yaratik_can=0;
		}
        int esya_sayisi = rand() % 2 + 1; 
        odalar[i].esya = (char *)malloc(100 * sizeof(char));
        odalar[i].esya[0] = '\0'; 
		int j =0;
        for (j = 0; j < esya_sayisi; j++) {
            int esya_turu = rand() % 3; 
            if (esya_turu == 0) {
                strcat(odalar[i].esya, "Kilic ");
            } else if (esya_turu == 1) {
                strcat(odalar[i].esya, "zirh ");
            }
        } 
    }
}
//Yaratilan odalari serbest birakan fonksiyon
void odalar_serbest_birak(Oda *odalar, int oda_sayisi) {
    int i=0;
    for (i = 0; i < oda_sayisi; i++) {
        free(odalar[i].aciklama);
        free(odalar[i].esya);
    }
}
//Odalardaki var olan esyalara gore aciklama yapan fonksiyon
void oda_bak(Oda *mevcut_oda) {
    printf("%s\n", mevcut_oda->aciklama);
    
    if (mevcut_oda->esya) {
        printf("Bu odada esya var:  %s\n", mevcut_oda->esya);
    }
    
    if (mevcut_oda->yaratik_var) {
        printf("Dikkatli ol yaratik var bu odada Sagligi da: %d\n", mevcut_oda->yaratik_can);
    }
    else {
    	printf("Sanslisin bu odada yaratik yok");
	}
}
//Oyuncunun envanterinde var olan esyalari gosteren fonksiyon
void envanter_goster(Oyuncu *oyuncu) {
     if (oyuncu->envanter_boyutu == 0) {
        printf("Envanterinizde hicbir sey yok.\n");
    } else {
        printf("Envanteriniz:\n");
        int i=0;
        for (i = 0; i < oyuncu->envanter_boyutu; i++) {
            printf("- %s\n", oyuncu->envanter[i]);
        }
    }
}
//Oyuncunun esya almasýný saglayan fonksiyon
void esya_al(Oyuncu *oyuncu, Oda *mevcut_oda) {
    if (mevcut_oda->esya && strlen(mevcut_oda->esya) > 0) {
        char *token = strtok(mevcut_oda->esya, " ");
        while (token != NULL) {
            
            oyuncu->envanter[oyuncu->envanter_boyutu] = (char *)malloc(strlen(token) + 1);
            strcpy(oyuncu->envanter[oyuncu->envanter_boyutu], token);
            oyuncu->envanter_boyutu++; //Aldigi esyaya göre envanteri artar
            printf("%s envanterinize eklendi.\n", token);

            //Envanterinde kilic varsa saldiri gucu artar
            if (strcmp(token, "Kilic") == 0) {
                oyuncu->guc += 20; 
                printf("Kilic alindi! Saldiri gucunuz %d oldu.\n", oyuncu->guc);
            } else if (strcmp(token, "Zirh") == 0) {
                oyuncu->guc += 15;  
                oyuncu->can += 45; // Envanterinde kilic varsa cani ve saldiri gucu artar
                if (oyuncu->can > 100) { 
                    oyuncu->can = 100;
                }
                printf("Zirh alindi! Saldiri gucunuz %d, caniniz %d oldu.\n", oyuncu->guc, oyuncu->can);
            }  

            token = strtok(NULL, " ");
        }
        mevcut_oda->esya[0] = '\0'; 
    } else {
        printf("Bu odada alacak bir sey yok.\n");
    }
}

// Odalar arasý yer degistirmeyi saglar
void odaya_git(int *mevcut_oda, int yon, int oda_sayisi, Oda *odalar) {
    if (yon >= 0 && yon < oda_sayisi) {
        *mevcut_oda = yon;
        printf("%d numarali odaya dogru hareket ediliyor.\n", yon);
        printf("%d numarali odadasiniz.\n", *mevcut_oda);

        
        Oda *gecici_oda = &odalar[*mevcut_oda];
        

        if (gecici_oda->esya && strlen(gecici_oda->esya) > 0) {
            printf("Bu odada esya var: %s\n", gecici_oda->esya);
        } else {
            printf("Bu odada esya bulunmuyor.\n");
        }

        if (gecici_oda->yaratik_var) {
            printf("Dikkat! Bu odada bir yaratik var. Cani: %d\n", gecici_oda->yaratik_can);
        } else {
            printf("Bu odada yaratik yok.\n");
        }
    } else {
        printf("Bu yone gidemezsiniz!\n");
    }
}


//oyuncunun envanterini kontrol etmesini saglar
void envanteri_kontrol_et(Oyuncu *oyuncu) {
    int kilic_var = 0, zirh_var = 0;
	int i =0;
    for (i = 0; i < oyuncu->envanter_boyutu; i++) {
        if (strcmp(oyuncu->envanter[i], "Kilic") == 0) {
            kilic_var = 1;
        } else if (strcmp(oyuncu->envanter[i], "Zirh") == 0) {
            zirh_var = 1;
        }
    }
    
}

//Yaratiga saldirmasini kontrol eden fonksiyon
void yaratiga_saldir(Oyuncu *oyuncu, Oda *mevcut_oda, int *mevcut_oda_index, int oda_sayisi) {
    if (mevcut_oda->yaratik_var) {
        printf("Yaratiga saldiriyorsunuz!\n");
        envanteri_kontrol_et(oyuncu);
        mevcut_oda->yaratik_can -= oyuncu->guc;
        
        if(mevcut_oda->yaratik_can>0){
        	printf("Yaratigin cani: %d \n", mevcut_oda->yaratik_can );
		}
        if (mevcut_oda->yaratik_can <= 0) {
            printf("Yaratik oldu.\n");
            mevcut_oda->yaratik_var = 0;
        } else {
            int canavar_atak_gucu = (rand() % 9) + 25;
            oyuncu->can -= canavar_atak_gucu;
            printf("Yaratik saldirdi! Caniniz: %d\n", oyuncu->can);
            if (oyuncu->can <= 0) {
                printf("Yenildiniz! Rastgele bir odaya yeniden dogdunuz...\n");
                oyuncu->can = 100;
                *mevcut_oda_index = rastgele_oda_sec(oda_sayisi);
            }
        }
    } else {
        printf("Burada saldiracak bir yaratik yok!\n");
    }
}
// Tum yaratiklarin olup olmadýgýný kontrol eden fonksiyon
int tum_yaratiklar_olduruldu_mu(Oda *odalar, int oda_sayisi) {
    int i;
    for (i = 0; i < oda_sayisi; i++) {
        if (odalar[i].yaratik_var) {
            return 0; 
        }
    }
    return 1; 
}
//Oyunun kaydedilmesini saglayan fonksiyon
void oyun_kaydet(Oyuncu *oyuncu, Oda *odalar, int mevcut_oda) {
    FILE *file = fopen("kayit.txt", "w");
    if (file == NULL) {
        printf("Dosya kaydedilemedi!\n");
        return;
    }
    fprintf(file, "%d %d %d %d\n", oyuncu->can, oyuncu->guc, oyuncu->envanter_boyutu, mevcut_oda);
    fclose(file);
    printf("Oyun kaydedildi.\n");
}
// Var olan oyunu yuklemeyi saglayan fonksiyon
void oyun_yukle(Oyuncu *oyuncu, Oda *odalar, int *mevcut_oda) {
    FILE *file = fopen("kayit.txt", "r");
    if (file) {
        fscanf(file, "%d %d %d %d", &oyuncu->can, &oyuncu->guc, &oyuncu->envanter_boyutu, mevcut_oda);
        fclose(file);
        printf("Oyun yuklendi.\n");
    } else {
        printf("Kaydedilmis oyun bulunamadi.\n");
    }
}
