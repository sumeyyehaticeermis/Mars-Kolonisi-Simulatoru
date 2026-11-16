# Mars Kolonisi Yaşam Destek Simülatörü

## Proje Hakkında
Bu proje, C programlama dili kullanılarak geliştirilmiş metin tabanlı bir hayatta kalma simülasyonudur.

Oyuncu, Mars'taki bir araştırma istasyonunda görev yapan mühendisi yöneterek kurtarma ekibi gelene kadar 15 gün hayatta kalmaya çalışır.

## Özellikler
- Üç farklı zorluk seviyesi (Kolay, Normal, Zor)
- Sağlık, enerji, oksijen ve su yönetimi
- Keşif ve kaynak toplama sistemi
- Oksijen üretimi
- Modül tamiri
- Dinlenme sistemi
- Rastgele kum fırtınası olayları
- Üç denemeli hava kilidi şifre sistemi
- Gün ve tur takibi
- Kazanma ve kaybetme koşulları
- Oyun sonu puan hesaplama

## Kullanılan Teknolojiler
- C Programlama Dili
- GCC Derleyicisi
- Visual Studio Code

## Kullanılan C Konuları
- Switch-Case
- If-Else
- For ve Do-While Döngüleri
- Break ve Continue
- Aritmetik ve Mantıksal Operatörler
- Rastgele Sayı Üretimi (rand ve srand)
- Fonksiyonlar
- Struct Veri Yapısı

## Kurulum ve Çalıştırma

Projeyi bilgisayarınıza indirdikten sonra terminalde:

```bash
gcc -Wall -Wextra -std=c11 mars_kolonisi.c -o mars_kolonisi
./mars_kolonisi