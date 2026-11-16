/* MARS KOLONISI YASAM DESTEK SIMULATORU - Algoritma ve Programlama */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

typedef struct {
    int saglik, enerji, oksijen, su, yedekParca, modulHasari;
    int kilitMuhurlu, gun, tur, firtinaTuru;
    int oyunBitti, kazanildi, cikisYapildi;
} Oyun;

int sinirla(int x, int alt, int ust) {
    if (x < alt) return alt;
    if (x > ust) return ust;
    return x;
}

void kaynaklariDengele(Oyun *o) {
    o->saglik = sinirla(o->saglik, 0, 100);
    o->enerji = sinirla(o->enerji, 0, 100);
    o->oksijen = sinirla(o->oksijen, 0, 100);
    o->modulHasari = sinirla(o->modulHasari, 0, 100);
    if (o->su < 0) o->su = 0;
    if (o->yedekParca < 0) o->yedekParca = 0;
}

void satiriTemizle(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

void durumRaporu(const Oyun *o) {
    printf("\n========== KOLONI DURUM RAPORU ==========\n");
    printf("Gun: %d | Tur: %d\n", o->gun, o->tur);
    printf("Saglik: %d/100\nEnerji: %d/100\nOksijen: %d/100\n",
           o->saglik, o->enerji, o->oksijen);
    printf("Su: %d birim\nYedek parca: %d\nModul hasari: %d/100\n",
           o->su, o->yedekParca, o->modulHasari);
    printf("Hava kilidi: %s\n", o->kilitMuhurlu ? "Muhurlu" : "Acik");
    if (o->oksijen < 25) printf("UYARI: Oksijen kritik seviyede!\n");
    if (o->saglik < 30) printf("UYARI: Saglik kritik seviyede!\n");
    if (o->enerji < 20) printf("UYARI: Enerji kritik seviyede!\n");
    if (o->modulHasari > 70) printf("UYARI: Modul agir hasarli!\n");
    if (o->oksijen >= 25 && o->saglik >= 30 && o->enerji >= 20 &&
        o->modulHasari <= 70) printf("TUM SISTEMLER NORMAL.\n");
    printf("=========================================\n");
}

/* G: Kesif */
int kesif(Oyun *o, int esikSu, int esikParca) {
    int zar, suEsigi;
    if (o->enerji < 15 || o->oksijen < 10) {
        printf("Kesif iptal: 15 enerji ve 10 oksijen gerekli.\n");
        return 0;
    }
    o->enerji -= 15;
    o->oksijen -= 10;
    printf("Kesif maliyeti: Enerji -15, oksijen -10.\n");
    if (o->enerji > 40 && o->oksijen > 30) suEsigi = esikSu;
    else suEsigi = esikSu / 2;

    zar = rand() % 100;
    printf("Su zari: %d / Esik: %d -> ", zar, suEsigi);
    if (zar < suEsigi) {
        int bulunanSu = 2 + rand() % 5;
        o->su += bulunanSu;
        printf("BASARILI, %d birim su bulundu.\n", bulunanSu);
    } else {
        zar = rand() % 100;
        printf("Yedek parca zari: %d / Esik: %d -> ", zar, esikParca);
        if (zar < esikParca) {
            o->yedekParca++;
            printf("BASARILI, 1 yedek parca bulundu.\n");
        } else printf("ELI BOS DONULDU.\n");
    }

    zar = rand() % 100;
    printf("Yaralanma zari: %d / Esik: 10 -> ", zar);
    if (zar < 10 || o->modulHasari > 70) {
        o->saglik -= 10;
        printf("YARALANMA! Saglik -10.\n");
    } else printf("YARALANMA YOK.\n");
    return 1;
}

/* O: Oksijen uretimi */
int oksijenUret(Oyun *o) {
    int miktar, kullanilan, uretilen;
    if (o->enerji < 5) {
        printf("Uretim iptal: En az 5 enerji gerekli.\n");
        return 0;
    }
    printf("Kullanilacak su miktari: ");
    if (scanf("%d", &miktar) != 1) {
        satiriTemizle();
        printf("Gecersiz miktar.\n");
        return 0;
    }
    if (miktar <= 0 || miktar > o->su) {
        printf("Gecersiz miktar. Depodaki su: %d\n", o->su);
        return 0;
    }
    kullanilan = miktar;
    if (kullanilan % 2 != 0) {
        kullanilan--;
        printf("Tek miktar girildi: 1 birim su depoda kaldi.\n");
    }
    if (kullanilan == 0) {
        printf("Uretim icin en az 2 birim su gerekli.\n");
        return 0;
    }
    uretilen = (kullanilan / 2) * 3;
    o->su -= kullanilan;
    o->oksijen += uretilen;
    o->enerji -= 5;
    if (o->oksijen > 100) o->oksijen = 100;
    printf("%d su kullanildi, %d oksijen uretildi. Enerji -5.\n",
           kullanilan, uretilen);
    return 1;
}

/* T: Tamir */
int tamirEt(Oyun *o) {
    int maliyet, onarim;
    if (o->yedekParca <= 0) {
        printf("Tamir yapilamadi: Yedek parca yok.\n");
        return 0;
    }
    if (o->enerji < 5) {
        printf("Tamir iptal: En az 5 enerji gerekli.\n");
        return 0;
    }
    if (o->modulHasari == 0) {
        printf("Modul zaten hasarsiz.\n");
        return 0;
    }
    if (o->modulHasari <= 30) {
        maliyet = 1; onarim = 20;
        printf("Hasar kademesi: HAFIF.\n");
    } else if (o->modulHasari <= 70) {
        maliyet = 1; onarim = 15;
        printf("Hasar kademesi: ORTA.\n");
    } else {
        maliyet = 2; onarim = 25;
        printf("Hasar kademesi: AGIR.\n");
    }
    if (o->yedekParca < maliyet) {
        printf("Yetersiz yedek parca. Gerekli: %d\n", maliyet);
        return 0;
    }
    o->yedekParca -= maliyet;
    o->modulHasari -= onarim;
    o->enerji -= 5;
    printf("Tamir tamamlandi: Hasar -%d, parca -%d, enerji -5.\n",
           onarim, maliyet);
    return 1;
}

/* U: Dinlenme */
int dinlen(Oyun *o) {
    o->enerji += 20;
    o->oksijen -= 5;
    printf("Dinlenme: Enerji +20, oksijen -5.\n");
    if (o->oksijen >= 20) {
        o->saglik += 5;
        printf("Oksijen yeterli: Saglik +5.\n");
    } else {
        o->saglik -= 5;
        printf("Oksijen 20'nin altinda: Saglik -5.\n");
    }
    return 1;
}

/* K: FOR, mod, continue ve break ile kum firtinasi */
int kumFirtinasi(Oyun *o) {
    int i, zar, hasarliTur = 0, toplamCan = 0, toplamOksijen = 0;
    printf("\nKUM FIRTINASI BASLADI! Sure: %d tur.\n", o->firtinaTuru);
    for (i = 1; i <= o->firtinaTuru; i++) {
        int carpan = (i % 2 == 0) ? 2 : 1;
        zar = rand() % 100;
        printf("Firtina turu %d, zar: %d / Esikler: 25, 40, 65\n", i, zar);
        if (o->kilitMuhurlu == 1 && zar >= 25 && zar < 40) {
            printf("Hava kilidi muhurlu; sizinti continue ile atlandi.\n");
            continue;
        }
        if (zar < 25) {
            o->saglik -= 5 * carpan;
            toplamCan += 5 * carpan;
            hasarliTur++;
            printf("Parca carpmasi: Saglik -%d.\n", 5 * carpan);
        } else if (zar < 40) {
            o->oksijen -= 8 * carpan;
            toplamOksijen += 8 * carpan;
            hasarliTur++;
            printf("Oksijen sizintisi: Oksijen -%d.\n", 8 * carpan);
        } else if (zar < 65) {
            o->modulHasari += 5 * carpan;
            hasarliTur++;
            printf("Modul hasari +%d.\n", 5 * carpan);
        } else printf("Koloni zarar gormeden korundu.\n");

        kaynaklariDengele(o);
        if (o->saglik == 0 || o->oksijen == 0) {
            printf("Kritik kaynak sifirlandi; break ile firtina durduruldu.\n");
            break;
        }
    }
    printf("Firtina ozeti: Hasarli tur=%d, saglik kaybi=%d, oksijen kaybi=%d.\n",
           hasarliTur, toplamCan, toplamOksijen);
    return 1;
}

/* H: Gun sayisindan uretilen sifre, en fazla 3 deneme */
int havaKilidi(Oyun *o) {
    char sifre = (char)('A' + (o->gun % 26));
    char giris = '\0';
    int deneme = 0;
    printf("Ipucu: Sifre A harfinden bugunun gun sayisi kadar ileridedir.\n");
    do {
        printf("Sifre (A-Z), deneme %d/3: ", deneme + 1);
        if (scanf(" %c", &giris) != 1) return 0;
        giris = (char)toupper((unsigned char)giris);
        deneme++;
        if (giris == sifre) {
            o->kilitMuhurlu = 1;
            printf("Dogru sifre! Hava kilidi muhurlendi.\n");
        } else {
            o->enerji -= 5;
            printf("Yanlis sifre. Enerji -5. ");
            if (giris < sifre) printf("Ileri git.\n");
            else printf("Geri git.\n");
        }
    } while (giris != sifre && deneme < 3);
    if (giris != sifre) {
        o->modulHasari += 20;
        printf("Uc deneme basarisiz. Modul hasari +20.\n");
    }
    return 1;
}

/* Her gecerli komut tur sayacini artirir; her 3 turda yeni gun baslar. */
void gunSonu(Oyun *o) {
    o->tur++;
    if (o->tur % 3 == 0) {
        if (o->gun == 15) {
            o->kazanildi = 1;
            o->oyunBitti = 1;
            printf("15. gun tamamlandi. Kurtarma ekibi koloniye ulasti!\n");
            return;
        }
        o->gun++;
        o->oksijen -= 8;
        printf("YENI GUN: %d. Gun baslangicinda oksijen -8.\n", o->gun);
        if (o->modulHasari > 70) {
            o->saglik -= 5;
            printf("Modul hasari 70'in ustunde: Saglik -5.\n");
        }
    }
    kaynaklariDengele(o);
    if (o->saglik == 0 || o->oksijen == 0) {
        o->oyunBitti = 1;
        o->kazanildi = 0;
        printf("Saglik veya oksijen sifirlandi. Simulasyon sona erdi.\n");
    }
}

void oyunSonu(const Oyun *o) {
    int puan = (o->gun * 100) + o->saglik + o->oksijen - o->modulHasari;
    printf("\n============== SONUC RAPORU ==============\n");
    if (o->kazanildi) printf("SONUC: KAZANDIN! Kurtarma ekibi geldi.\n");
    else if (o->cikisYapildi) printf("SONUC: Q komutuyla cikis yapildi.\n");
    else printf("SONUC: KAYBETTIN! Yasam destek kosullari tukendi.\n");
    printf("Hayatta kalinan gun: %d\nToplam tur: %d\n", o->gun, o->tur);
    printf("Saglik: %d | Enerji: %d | Oksijen: %d\n",
           o->saglik, o->enerji, o->oksijen);
    printf("Su: %d | Yedek parca: %d | Modul hasari: %d\n",
           o->su, o->yedekParca, o->modulHasari);
    printf("Puan: %d\n==========================================\n", puan);
}

int main(void) {
    Oyun oyun = {100, 80, 70, 12, 2, 10, 0, 1, 0, 5, 0, 0, 0};
    char zorluk, komut;
    int esikSu = 45, esikParca = 25, zorlukSecildi = 0;
    srand((unsigned int)time(NULL));

    printf("============================================\n");
    printf("       MARS KOLONISI YASAM DESTEK SIMULATORU\n");
    printf("============================================\n");

    do {
        printf("Zorluk secin (K: Kolay, N: Normal, Z: Zor): ");
        if (scanf(" %c", &zorluk) != 1) return 1;
        switch (zorluk) {
            case 'k':
            case 'K':
                oyun.saglik = 100; oyun.enerji = 100; oyun.oksijen = 85;
                oyun.su = 18; oyun.yedekParca = 3; oyun.modulHasari = 5;
                oyun.firtinaTuru = 3; esikSu = 60; esikParca = 40;
                zorlukSecildi = 1;
                printf("Kolay zorluk secildi.\n");
                break;
            case 'n':
            case 'N':
                oyun.saglik = 100; oyun.enerji = 80; oyun.oksijen = 70;
                oyun.su = 12; oyun.yedekParca = 2; oyun.modulHasari = 10;
                oyun.firtinaTuru = 5; esikSu = 45; esikParca = 25;
                zorlukSecildi = 1;
                printf("Normal zorluk secildi.\n");
                break;
            case 'z':
            case 'Z':
                oyun.saglik = 85; oyun.enerji = 65; oyun.oksijen = 60;
                oyun.su = 8; oyun.yedekParca = 1; oyun.modulHasari = 20;
                oyun.firtinaTuru = 7; esikSu = 30; esikParca = 15;
                zorlukSecildi = 1;
                printf("Zor zorluk secildi.\n");
                break;
            default:
                printf("Gecersiz zorluk! K, N veya Z giriniz.\n");
                break;
        }
    } while (zorlukSecildi == 0);

    printf("\nSimulasyon basladi. 15. gun tamamlaninca kurtarma ekibi gelecek.\n");
    durumRaporu(&oyun);

    do {
        int islemBasarili = 0;
        printf("\nKomutlar: G-Kesif O-Oksijen T-Tamir D-Durum ");
        printf("U-Dinlen K-Firtina H-Kilit Q-Cikis\nKomutunuz: ");
        if (scanf(" %c", &komut) != 1) {
            oyun.cikisYapildi = 1;
            oyun.oyunBitti = 1;
            break;
        }

        switch (komut) {
            case 'g':
            case 'G':
                islemBasarili = kesif(&oyun, esikSu, esikParca);
                break;
            case 'o':
            case 'O':
                islemBasarili = oksijenUret(&oyun);
                break;
            case 't':
            case 'T':
                islemBasarili = tamirEt(&oyun);
                break;
            case 'd':
            case 'D':
                durumRaporu(&oyun);
                break;
            case 'u':
            case 'U':
                islemBasarili = dinlen(&oyun);
                break;
            case 'k':
            case 'K':
                islemBasarili = kumFirtinasi(&oyun);
                break;
            case 'h':
            case 'H':
                if (oyun.kilitMuhurlu) printf("Hava kilidi zaten muhurlu.\n");
                else islemBasarili = havaKilidi(&oyun);
                break;
            case 'q':
            case 'Q':
                oyun.cikisYapildi = 1;
                oyun.oyunBitti = 1;
                oyun.tur++;
                break;
            default:
                printf("Tanimlanmayan komut! Tur harcanmadi.\n");
                break;
        }

        if (islemBasarili) {
            kaynaklariDengele(&oyun);
            gunSonu(&oyun);
        }
    } while (oyun.oyunBitti == 0);

    oyunSonu(&oyun);
    return 0;
}
