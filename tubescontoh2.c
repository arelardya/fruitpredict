#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define maks_pengguna 50
#define maks_klien 50
#define DATA_FILE "data.txt"

char adminuser[50] = "admin";
char adminpass[50] = "kemenkeu10";

struct KepalaDaerah {
    char nama[50], NIK[17], email[50], katasandi[50], ttl[20], jabatan[20], daerah[20], statusPermintaanStr[50];
    long long danaPerekonomian, pemasukkan, pengeluaran, permintaanBantuan, belanjaProduk;
    int statusPermintaan; 
    struct {
        char namaProduk[50];
        int jumlah;
        double hargaPerItem;
    } produkKeranjang[5]; 
    int jumlahProduk;       
};

struct SektorIndustri {
    char nama[50], username[50], sektorindustri[50], katasandi[10];
    double *pemasukkan; 
    double *pengeluaran; 
    int jumlahPemasukkan;
    int jumlahPengeluaran;
    double danaTotal;
    struct {
        char namaProduk[50];
        int jumlah;
        double hargaPerItem;
    } stokProduk[5];
    int jumlahProduk;
};

struct TabunganNegara {
    long long saldo;
    struct {
        char deskripsi[100];
        long long jumlah;
    } pemasukan[100], pengeluaran[100];  
    int jumlahPemasukan;
    int jumlahPengeluaran;
};

struct KepalaDaerah listKepda[maks_pengguna];
struct SektorIndustri listSekdus[maks_klien];
struct TabunganNegara negara;  
float pajakNegara = 0.0;  
int jumlahpengguna = 0;    
int jumlahklien = 0;       
int jumlahSektorIndustri = 0;


void PulihData();
void SimpanData();
void LoginAdmin();
void MenuKemenkeu();
void DataKepalaDaerah();
void DataSektorIndustri();
void PendistribusianDana();
void PengaturanPajak();
void KepalaDaerah();
void LoginPengguna();
void MenuPengguna(int i);
void SektorIndustri();
void LoginKlien();
void MenuKlien(int i);
void AlurPerekonomian(int i);
void PengajuanDanaBantuan(int i);
void DistribusiKemenkeu(int i);
void KerjasamaSektorIndustri(int i);
void AlurEkonomiSekdus(int i);
void PengaturanProduksi(int i);
void PengaturanKerjasamaKepda(int i);
void PengaturanKerjaSama();
void DistribusiPerdagangan();
int validasiEmail(char *email);
int validasiTanggal(char *tanggal);

int main() {
    system("cls");
    PulihData();
    int pilihan;

    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda berada dalam menu: Home Menu\n\n");
    printf("::   Anda dipersilahkan untuk memilih SATU dari TIGA opsi berikut ini!   ::\n");
    printf("::                Pastikan peran yang Anda pilih sesuai!                 ::\n");
    printf("[1] Admin    :: Kementerian Keuangan\n");
    printf("[2] Pengguna :: Kepala Daerah\n");
    printf("[3] Klien    :: Sektor Industri\n");
    printf("Pilihan anda: ");
    scanf("%d", &pilihan);
    getchar(); 

    switch(pilihan) {
        case 1:
            LoginAdmin();
            break;
        case 2:
            KepalaDaerah();
            break;
        case 3:
            LoginKlien();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            main();
            break;
    }
    SimpanData();
    for (int i = 0; i < jumlahklien; i++) {
        free(listSekdus[i].pemasukkan);
        free(listSekdus[i].pengeluaran);
    }
    return 0;
}

void PulihData() {
    FILE *file = fopen(DATA_FILE, "r");
    if (!file) return;
    while (fscanf(file, "%s,%s,%s,%s,%s,%s,%s,%lld,%lf,%lf\n",
                  listKepda[jumlahpengguna].nama,
                  listKepda[jumlahpengguna].NIK,
                  listKepda[jumlahpengguna].email,
                  listKepda[jumlahpengguna].katasandi,
                  listKepda[jumlahpengguna].ttl,
                  listKepda[jumlahpengguna].jabatan,
                  listKepda[jumlahpengguna].daerah,
                  &listKepda[jumlahpengguna].danaPerekonomian,
                  &listKepda[jumlahpengguna].pemasukkan,
                  &listKepda[jumlahpengguna].pengeluaran) != EOF) {
        jumlahpengguna++;
    }
    while (fscanf(file, "%s,%s,%s,%s,%lf,%d,%d\n",
                  listSekdus[jumlahklien].nama,
                  listSekdus[jumlahklien].username,
                  listSekdus[jumlahklien].sektorindustri,
                  listSekdus[jumlahklien].katasandi,
                  &listSekdus[jumlahklien].danaTotal,
                  &listSekdus[jumlahklien].jumlahPemasukkan,
                  &listSekdus[jumlahklien].jumlahPengeluaran) != EOF) {
        listSekdus[jumlahklien].pemasukkan = malloc(sizeof(double) * listSekdus[jumlahklien].jumlahPemasukkan);
        listSekdus[jumlahklien].pengeluaran = malloc(sizeof(double) * listSekdus[jumlahklien].jumlahPengeluaran);

        for (int j = 0; j < listSekdus[jumlahklien].jumlahPemasukkan; j++) {
            fscanf(file, "%lf\n", &listSekdus[jumlahklien].pemasukkan[j]);
        }
        for (int j = 0; j < listSekdus[jumlahklien].jumlahPengeluaran; j++) {
            fscanf(file, "%lf\n", &listSekdus[jumlahklien].pengeluaran[j]);
        }
        jumlahklien++;
    }
    fclose(file);
}

void SimpanData() {
    FILE *file = fopen(DATA_FILE, "w");
    if (!file) return;
    
    for (int i = 0; i < jumlahpengguna; i++) {
        fprintf(file, "%s,%s,%s,%s,%s,%s,%s,%lld,%lld,%lld\n",
                listKepda[i].nama,
                listKepda[i].NIK,
                listKepda[i].email,
                listKepda[i].katasandi,
                listKepda[i].ttl,
                listKepda[i].jabatan,
                listKepda[i].daerah,
                listKepda[i].danaPerekonomian,
                listKepda[i].pemasukkan,
                listKepda[i].pengeluaran);
    }
    for (int i = 0; i < jumlahklien; i++) {
        fprintf(file, "%s,%s,%s,%s,%lf,%d,%d\n",
                listSekdus[i].nama,
                listSekdus[i].username,
                listSekdus[i].sektorindustri,
                listSekdus[i].katasandi,
                listSekdus[i].danaTotal,
                listSekdus[i].jumlahPemasukkan,
                listSekdus[i].jumlahPengeluaran);
        for (int j = 0; j < listSekdus[i].jumlahPemasukkan; j++) {
            fprintf(file, "%lf\n", listSekdus[i].pemasukkan[j]);
        }
        for (int j = 0; j < listSekdus[i].jumlahPengeluaran; j++) {
            fprintf(file, "%lf\n", listSekdus[i].pengeluaran[j]);
        }
    }
    fclose(file);
}

void LoginAdmin() {
    char username[50], password[50];
    int kesempatan = 3;

    while(kesempatan > 0) {
        system("cls");
        printf("Silakan untuk memasukkan kredensial Anda seperti berikut:\n");
        printf("> Username   (maks 50 karakter)\n");
        printf("> Kata Sandi (6-10 karakter)\n");
        printf("Username : ");
        gets(username);

        printf("Password : ");
        gets(password);

        if (strlen(username) == 0 || strlen(username) > 50) {
            printf("Username tidak valid! Panjang maksimal adalah 50 karakter.\n");
            continue;
        }
        if (strlen(password) < 6 || strlen(password) > 10) {
            printf("Password tidak valid! Panjang harus 6-10 karakter.\n");
            continue;
        }
        if(strcmp(adminuser, username) == 0 && strcmp(adminpass, password) == 0) {
            printf("Login Berhasil!\n");
            printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
            getchar(); 
            MenuKemenkeu();
            return;
        } else {
            kesempatan--;
            printf("Nama Pengguna atau Kata Sandi salah!\n");
            printf("Kesempatan habis. Silahkan coba lagi nanti.\n");
            printf("\nTekan tombol [ENTER] untuk melanjutkan...\n"); 
            getchar(); 
        }
    }
    main();
}

void MenuKemenkeu() {
    int pilihan;
    
    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda berhasil log in sebagai : Admin :: Kementerian Keuangan.\n");
    printf("\n\n");
    printf("Pilihan Menu :\n");
    printf("[1] Pajak dan Tabungan Negara\n");
    printf("[2] Data Kepala Daerah\n");
    printf("[3] Data Sektor Industri\n");
    printf("[4] Pendistribusian Pendapatan atau Bantuan Dana\n");
    printf("[5] Pengajuan Kerjasama dengan Kepala Daerah\n");
    printf("[6] Distribusi Perdagangan Internasional\n");
    printf("[7] Keluar\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    switch(pilihan) {
        case 1:
            PengaturanPajak();
            break;
        case 2:
            DataKepalaDaerah();
            break;
        case 3:
            DataSektorIndustri();
            break;
        case 4:
            PendistribusianDana();
            break;
        case 5:
            PengaturanKerjaSama();
            break;
        case 6 :
            DistribusiPerdagangan();
            break;
        case 7:
            main();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            MenuKemenkeu();
            break;
    }
}

void PengaturanPajak() {
    int pilihan;

    system("cls");
    printf("[1] Ubah besar Pajak Negara\n");
    printf("[2] Lihat Alur Perekonomian Negara (Rp)\n");
    printf("[3] Kembali ke Menu Utama\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);

    switch (pilihan) {
        case 1: {
            float pajakBaru;
            printf("Masukkan besar Pajak Negara baru (dalam %%): ");
            scanf("%f", &pajakBaru);
            if (pajakBaru >= 0 && pajakBaru <= 100) {
                pajakNegara = pajakBaru;
                printf("Pajak Negara berhasil diperbarui menjadi %.2f%%\n", pajakNegara);
            } else {
                printf("ERROR! Pajak tidak valid! Masukkan nilai antara 0 hingga 100.\n");
            }
            break;
        }
        case 2:
            printf(":: Tabungan Negara (Rp): Rp%lld\n", negara.saldo);
            printf("\nPemasukan:\n");
            if (negara.jumlahPemasukan > 0) {
                for (int i = 0; i < negara.jumlahPemasukan; i++) {
                    printf("Pemasukan %d: %s, Rp%lld\n", i + 1, negara.pemasukan[i].deskripsi, negara.pemasukan[i].jumlah);
                }
            } else {
                printf("Belum ada pemasukan.\n");
            }
            printf("\nPengeluaran:\n");
            if (negara.jumlahPengeluaran > 0) {
                for (int i = 0; i < negara.jumlahPengeluaran; i++) {
                    printf("Pengeluaran %d: %s, Rp%lld\n", i + 1, negara.pengeluaran[i].deskripsi, negara.pengeluaran[i].jumlah);
                }
            } else {
                printf("Belum ada pengeluaran.\n");
            }
            break;
        case 3:
            MenuKemenkeu();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            PengaturanPajak();
            break;
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKemenkeu();
}

void DataKepalaDaerah() {
    system("cls");
    if (jumlahpengguna == 0) {
        printf("Belum ada Kepala Daerah yang terdaftar.\n");
    } else {
        for (int i = 0; i < jumlahpengguna; i++) {
            printf("Nama: %s\n", listKepda[i].nama);
            printf("Daerah: %s\n", listKepda[i].daerah);
            printf("Pendapatan: Rp%lld\n", listKepda[i].pemasukkan);
            printf("Pengeluaran: Rp%lld\n\n", listKepda[i].pengeluaran);
        }
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKemenkeu();
}

void DataSektorIndustri() {
    system("cls");
    if (jumlahklien == 0) {
        printf("Belum ada Sektor Industri yang terdaftar.\n");
    } else {
        for (int i = 0; i < jumlahklien; i++) {
            printf("Nama: %s\n", listSekdus[i].nama);
            printf("Sektor Industri: %s\n", listSekdus[i].sektorindustri);
            printf("Penghasilan: Rp%.2f\n\n", listSekdus[i].danaTotal);
        }
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n"); 
    getchar();
    MenuKemenkeu();
}

void PendistribusianDana() {
    system("cls");
    int pilihan;
    long long danaDiberikan;

    printf("Pendistribusian Dana - Silakan pilih Kepala Daerah:\n");
    for (int i = 0; i < jumlahpengguna; i++) {
        printf("[%d] %s (%s) - Permintaan: %s\n", 
               i + 1, listKepda[i].nama, listKepda[i].daerah, 
               listKepda[i].statusPermintaan == 1 ? "SEDANG DIPROSES" : 
               (listKepda[i].statusPermintaan == 2 ? "PERMINTAAN DISETUJUI" : "-"));
    }
    printf("[0] Kembali ke Menu Utama\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    if (pilihan == 0) {
        MenuKemenkeu(); 
        return;
    }

    if (pilihan > 0 && pilihan <= jumlahpengguna) {
        int index = pilihan - 1;

        if (listKepda[index].statusPermintaan == 1) {
            long long danaDiajukan = listKepda[index].permintaanBantuan;
            printf("Distribusi Dana : Rp");
            scanf("%lld", &danaDiberikan);
            
            if (danaDiberikan > negara.saldo) {
                printf("ERROR! Dana negara tidak mencukupi!\n");
            } else {
                negara.saldo -= danaDiberikan;  
                listKepda[index].danaPerekonomian += danaDiberikan;
                listKepda[index].pemasukkan += danaDiberikan;  
                listKepda[index].statusPermintaan = 2; 
                strcpy(listKepda[index].statusPermintaanStr, "PERMINTAAN DISETUJUI");

                printf("\nDana sebesar Rp%lld berhasil didistribusikan ke %s.\n", danaDiberikan, listKepda[index].daerah);
            }
        } else {
            printf("\nTidak ada permintaan dana yang sedang diajukan oleh %s.\n", listKepda[index].daerah);
        }
    } else {
        printf("\nERROR! Pilihan tidak valid!\n");
    }

    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKemenkeu();  
}

void PengaturanKerjaSama() {
    system("cls");
    int pilihan;
    float hargaBarang;

    if (jumlahpengguna == 0) {
        printf("Belum ada Kepala Daerah yang terdaftar.\n");
    } else {
        printf("Pengaturan Kerja Sama - Silakan pilih Kepala Daerah yang mengajukan pembelian barang:\n");
        for (int i = 0; i < jumlahpengguna; i++) {
            printf("[%d] %s (%s)\n", i + 1, listKepda[i].nama, listKepda[i].daerah);
        }
        printf("[0] Kembali ke Menu Utama\n");

        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);

        if (pilihan == 0) {
            MenuKemenkeu();
        } else if (pilihan > 0 && pilihan <= jumlahpengguna) {
            printf("Masukkan harga barang yang diajukan (Rp): ");
            scanf("%f", &hargaBarang);

            if (hargaBarang > negara.saldo) {
                printf("ERROR! Dana tidak mencukupi untuk pembelian barang!\n");
            } else {
                negara.saldo -= hargaBarang;
                listKepda[pilihan - 1].pengeluaran += hargaBarang;
                listKepda[pilihan - 1].danaPerekonomian -= hargaBarang;
                strcpy(negara.pengeluaran[negara.jumlahPengeluaran].deskripsi, "Pembelian Barang dari Kepala Daerah");
                negara.pengeluaran[negara.jumlahPengeluaran].jumlah = hargaBarang;
                negara.jumlahPengeluaran++;

                printf("Barang berhasil dibeli dari %s dengan harga Rp%.2f.\n", listKepda[pilihan - 1].nama, hargaBarang);
            }
        } else {
            printf("ERROR! Pilihan tidak valid!\n");
        }
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKemenkeu();
}

void DistribusiPerdagangan() {
    system("cls");
    int pilihan;
    float hargaJual;

    if (jumlahpengguna == 0) {
        printf("Belum ada Kepala Daerah yang terdaftar.\n");
    } else {
        printf("Distribusi Perdagangan - Silakan pilih Kepala Daerah untuk menjual barang:\n");
        for (int i = 0; i < jumlahpengguna; i++) {
            printf("[%d] %s (%s)\n", i + 1, listKepda[i].nama, listKepda[i].daerah);
        }
        printf("[0] Kembali ke Menu Utama\n");

        printf("Pilihan Anda: ");
        scanf("%d", &pilihan);

        if (pilihan == 0) {
            MenuKemenkeu(); 
        } else if (pilihan > 0 && pilihan <= jumlahpengguna) {
            printf("Masukkan harga jual barang (Rp): ");
            scanf("%f", &hargaJual);

            negara.saldo += hargaJual;
            listKepda[pilihan - 1].pemasukkan += hargaJual;
            listKepda[pilihan - 1].danaPerekonomian += hargaJual;

            strcpy(negara.pemasukan[negara.jumlahPemasukan].deskripsi, "Penjualan Barang ke Pihak Internasional");
            negara.pemasukan[negara.jumlahPemasukan].jumlah = hargaJual;
            negara.jumlahPemasukan++;

            printf("Barang berhasil dijual dari %s dengan harga Rp%.2f.\n", listKepda[pilihan - 1].nama, hargaJual);
        } else {
            printf("ERROR! Pilihan tidak valid!\n");
        }
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKemenkeu(); 
}

void KepalaDaerah() {
    struct KepalaDaerah kepda;
    int pilihan;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda berada dalam lobi: User :: Kepala Daerah.\n");
    printf("Anda dipersilakan untuk memilih satu dari tiga opsi berikut:\n");
    printf("[1] Registrasi  (Sign-up)\n");
    printf("[2] Masuk       (Sign-in/Login)\n");
    printf("[3] Keluar\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar(); 
    system("cls");

    switch(pilihan) {
        case 1:
            printf("Silakan untuk memasukkan TUJUH (7) kredensial baru Anda seperti berikut:\n");
            printf("[1] Nama Lengkap         (maks 50 karakter)\n");
            printf("[2] NIK                  (wajib 16 digit)\n");
            printf("[3] Email                (memenuhi sintaks email sesungguhnya)\n");
            printf("[4] Kata Sandi           (8-10 karakter)\n");
            printf("[5] Tempat/Tanggal Lahir (Tempat, DD/MM/YYYY)\n");
            printf("[6] Jabatan              (Gubernur/Bupati/Walikota)\n");
            printf("[7] Daerah Perwakilan    (nama daerah yang dipimpin)\n");

            do {
                printf("\nNama Lengkap : ");
                gets(kepda.nama);
                if (strlen(kepda.nama) > 50) {
                    printf("Nama terlalu panjang, maksimal 50 karakter...\n");
                }
            } while (strlen(kepda.nama) > 50);
            
            do {
                printf("NIK : ");
                gets(kepda.NIK);
                if (strlen(kepda.NIK) != 16) {
                    printf("NIK harus terdiri dari 16 digit...\n");
                }
            } while (strlen(kepda.NIK) != 16);
            
            do {
                printf("Email: ");
                gets(kepda.email);
                if (!validasiEmail(kepda.email)) {
                    printf("Email tidak valid. Harus mengandung '@' dan '.'...\n");
                }
            } while (!validasiEmail(kepda.email));
            
            do {
                printf("Kata Sandi (8-10 karakter): ");
                gets(kepda.katasandi);
                if (strlen(kepda.katasandi) < 8 || strlen(kepda.katasandi) > 10) {
                    printf("Kata sandi harus antara 8 hingga 10 karakter.\n");
                }
            } while (strlen(kepda.katasandi) < 8 || strlen(kepda.katasandi) > 10);
            
            do {
                printf("Tempat/Tanggal Lahir (Tempat, DD/MM/YYYY): ");
                gets(kepda.ttl);
                if (!validasiTanggal(kepda.ttl)) {
                    printf("Format tanggal tidak valid. Harus DD/MM/YYYY.\n");
                }
            } while (!validasiTanggal(kepda.ttl));
            
            do {
                printf("Jabatan (Gubernur/Bupati/Walikota): ");
                gets(kepda.jabatan);
                if (strcmp(kepda.jabatan, "Gubernur") != 0 && strcmp(kepda.jabatan, "gubernur") != 0 && strcmp(kepda.jabatan, "Bupati") != 0 && strcmp(kepda.jabatan, "bupati") != 0 && strcmp(kepda.jabatan, "Walikota") != 0 && strcmp(kepda.jabatan, "walikota") != 0) {
                    printf("Jabatan harus salah satu dari (Gubernur/Bupati/Walikota).\n");
                }
            } while (strcmp(kepda.jabatan, "Gubernur") != 0 && strcmp(kepda.jabatan, "gubernur") != 0 && strcmp(kepda.jabatan, "Bupati") != 0 && strcmp(kepda.jabatan, "bupati") != 0 && strcmp(kepda.jabatan, "Walikota") != 0 && strcmp(kepda.jabatan, "walikota") != 0);
            
            do {
                printf("Daerah Perwakilan: ");
                gets(kepda.daerah);
                if (strlen(kepda.daerah) == 0) {
                    printf("Daerah Perwakilan tidak boleh kosong.\n");
                }
            } while (strlen(kepda.daerah) == 0);

            kepda.danaPerekonomian = 1000000000000;
            kepda.pemasukkan = 0;
            kepda.pengeluaran = 0;

            listKepda[jumlahpengguna++] = kepda;
            system("cls");
            printf("\nRegistrasi berhasil!\n");
            printf("INFO: Silakan untuk melanjutkan interaksi berikut dengan masuk ke menu sign-in/login...\n");
            printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
            getchar();
            LoginPengguna();
            break;
        case 2:
            LoginPengguna();
            break;
        case 3:
            main();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            KepalaDaerah();
            break;
    }
}

void LoginPengguna() {
    char NIK[17], password[50];
    int i = 0;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda berada dalam lobi: User :: Kepala Daerah.\n");
    printf("\nSilakan untuk memasukkan DUA (2) kredensial Anda seperti berikut\n");
    printf("1. NIK:        (wajib 16 digit)\n");
    printf("2. Kata Sandi: (8-16 karakter)\n");
    printf("\nNIK        (16 digit)     : ");
    gets(NIK);
    printf("Kata Sandi (8-16 karakter): ");
    gets(password);

    while(i < jumlahpengguna) {
        if(strcmp(listKepda[i].NIK, NIK) == 0 && strcmp(listKepda[i].katasandi, password) == 0) {
            printf("Login Berhasil!\n");
            MenuPengguna(i);
            return;
        }
        i++;
    }
    printf("ERROR! NIK atau password salah!\n");
    KepalaDaerah();
}

void MenuPengguna(int i) {
    int pilihan;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: Kepala Daerah: %s, %s %s.\n\n", listKepda[i].nama, listKepda[i].jabatan, listKepda[i].daerah);
    printf("Pilihan Menu:\n");
    printf("[1] Alur Perekonomian Dana Daerah\n");
    printf("[2] Pengajuan Dana Bantuan dari Pemerintah Pusat\n");
    printf("[3] Distribusi Perdagangan kepada Pemerintah Pusat\n");
    printf("[4] Pengajuan Kerjasama dengan Sektor Industri\n");
    printf("[5] Keluar.. (Log Out)\n");
    printf("\nAnda dipersilakan untuk memilih dari EMPAT (5) opsi di atas...\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    switch(pilihan) {
        case 1:
        AlurPerekonomian(i);
        break;
        case 2:
        PengajuanDanaBantuan(i);
        break;
        case 3:
        DistribusiKemenkeu(i);
        break;
        case 4:
        KerjasamaSektorIndustri(i);
        break;   
        case 5:
        KepalaDaerah();
        break;
        default:
        printf("ERROR! Input yang diterima tidaklah valid!\n");
        printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
        MenuPengguna(i);
        break;
    }
}

void AlurPerekonomian(int i) {
    system("cls");
 
    if (listKepda[i].danaPerekonomian == 0) {
        listKepda[i].danaPerekonomian = 1000000000000;
    }

    listKepda[i].danaPerekonomian += listKepda[i].pemasukkan - listKepda[i].pengeluaran;

    printf("\nMenu: [1] Lihat Alur Perekonomian Dana Daerah\n");
    printf("Berikut adalah data akumulasi perekonomian dana daerah dari yang bersangkutan:\n");
    printf("====================================================================================================\n");

    if (listKepda[i].pemasukkan > 0) {
        printf(":: Dana Perekonomian Total Daerah: Rp%lld >> (+Rp%lld)\n", listKepda[i].danaPerekonomian, listKepda[i].pemasukkan);
    } else {
        printf(":: Dana Perekonomian Total Daerah: Rp%lld\n",listKepda[i].danaPerekonomian);
    }
    
    printf("\nPemasukkan  (Incomes) :\n");
    printf("Rp%lld\n", listKepda[i].pemasukkan);
    printf("\nPengeluaran (Expenses):\n");
    printf("Rp%lld\n", listKepda[i].pengeluaran);
    printf("====================================================================================================\n");
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuPengguna(i);
}

void PengajuanDanaBantuan(int i) {
    system("cls");
    char pilihan;
    long long danaDiajukan;

    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: User :: Kepala Daerah.\n");
    printf("... Profil Kepala Daerah: %s, %s %s.\n\n", listKepda[i].nama, listKepda[i].jabatan, listKepda[i].daerah);

    printf("Menu: [2] Pengajuan Dana Bantuan dari Kementerian Keuangan\n");
    printf("====================================================================================================\n");
    printf("Berikut adalah status permintaan bantuan dana Anda saat ini:\n");

    if (listKepda[i].statusPermintaan == 0) {
        printf(":: Status Permintaan: -\n");
    } else if (listKepda[i].statusPermintaan == 1) {
        printf(":: Status Permintaan: SEDANG DIPROSES\n");
    } else if (listKepda[i].statusPermintaan == 2) {
        printf(":: Status Permintaan: PERMINTAAN TELAH DISETUJUI\n");
    }
    printf("====================================================================================================\n");

    do {
        printf("\nAjukan permintaan bantuan dana kepada pihak Kemenkeu?\n");
        printf("(ketik Y atau y jika hendak mengajukan)\n");
        printf("(ketik Q atau q untuk keluar...)\n");
        printf("Pilihan Anda: ");
        scanf(" %c", &pilihan);
        getchar();

        if (pilihan == 'Y' || pilihan == 'y') {
            if (listKepda[i].statusPermintaan == 0) { 
                printf("Masukkan jumlah dana yang ingin diajukan: ");
                scanf("%lld", &danaDiajukan);
                listKepda[i].permintaanBantuan = danaDiajukan;
                listKepda[i].statusPermintaan = 1;
                strcpy(listKepda[i].statusPermintaanStr, "SEDANG DIPROSES");
                printf("\nPermintaan Anda telah diajukan.\nMenunggu persetujuan dari pihak Kemenkeu...\n");
                printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
                getchar();
                MenuPengguna(i);
                return;
            } else {
                printf("Permintaan sudah diajukan sebelumnya.\n");
                printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
                MenuPengguna(i);
                getchar();
            }
        } else if (pilihan == 'Q' || pilihan == 'q') {
            MenuPengguna(i);
            return;
        } else {
            printf("Pilihan tidak valid! Silakan masukkan pilihan yang benar.\n");
        }
    } while (pilihan != 'Y' && pilihan != 'y' && pilihan != 'Q' && pilihan != 'q');
}

void DistribusiKemenkeu(int i) {
    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: User :: Kepala Daerah.\n");
    printf("... Profil Kepala Daerah: %s, %s %s.\n\n", listKepda[i].nama, listKepda[i].jabatan, listKepda[i].daerah);
    printf("Menu: [3] Distribusi kepada Kementerian Keuangan\n");
    printf("====================================================================================================\n");

    if (listKepda[i].jumlahProduk == 0) {
        printf("Daftar keranjang pesanan produk Anda dari Sektor Industri tengah KOSONG!\n");
    } else {
        printf("Produk yang siap didistribusikan:\n");
        for (int j = 0; j < listKepda[i].jumlahProduk; j++) {
            printf("[%d] %s - Jumlah: %d, Total Harga: Rp%.2f\n", 
                   j + 1, listKepda[i].produkKeranjang[j].namaProduk, 
                   listKepda[i].produkKeranjang[j].jumlah, 
                   listKepda[i].produkKeranjang[j].jumlah * listKepda[i].produkKeranjang[j].hargaPerItem);
        }
    }
    printf("====================================================================================================\n");
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
}

void KerjasamaSektorIndustri(int i) {
    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: User :: Kepala Daerah.\n");
    printf("... Profil Kepala Daerah: %s, %s %s.\n\n", listKepda[i].nama, listKepda[i].jabatan, listKepda[i].daerah);
    printf("Menu: [4] Pengaturan Kerja Sama dengan Sektor Industri\n");
    printf("====================================================================================================\n");
    printf("Berikut adalah data dari seluruh Sektor Industri yang tersedia...\n");
    for (int j = 0; j < jumlahSektorIndustri; j++) {
        printf("[%03d] Nama Lengkap: %s\n", j + 1, listSekdus[j].nama);
        printf("... Nama Industri: %s\n", listSekdus[j].sektorindustri);
        printf("====================================================================================================\n");
    }
    printf("(ketik angka 0 untuk kembali ke menu utama Kepala Daerah)\n");
    printf("> Pilihan Anda: ");
    getchar();
}

void SektorIndustri() {
    struct SektorIndustri sekdus;
    int pilihan;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda berada dalam lobi: Klien :: Sektor Industri.\n");
    printf("Anda dipersilakan untuk memilih satu dari tiga opsi berikut:\n");
    printf("[1] Registrasi  (Sign-up)\n");
    printf("[2] Masuk       (Sign-in/Login)\n");
    printf("[3] Keluar\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    switch(pilihan) {
        case 1:
            printf("Silakan untuk memasukkan EMPAT (4) kredensial baru Anda seperti berikut:\n");
            printf("[1] Nama Lengkap         (maks 50 karakter)\n");
            printf("[2] Nama Pengguna        (atau username)\n");
            printf("[3] Nama Sektor Industri\n");
            printf("[4] Kata Sandi           (8-10 karakter)\n");

            printf("Nama Lengkap: ");
            gets(sekdus.nama);
            if(strlen(sekdus.nama) == 0 || strlen(sekdus.nama) > 50) {
                printf("Nama Lengkap tidak valid! Harus 1-50 karakter.\n");
                return;
            }
            printf("Nama Pengguna: ");
            gets(sekdus.username);
            sekdus.username[strcspn(sekdus.username, "\n")] = '\0';
            if(strlen(sekdus.username) == 0 || strlen(sekdus.username) > 30) {
                printf("Nama Pengguna tidak valid! Harus 1-30 karakter.\n");
                return;
            }
            printf("Nama Sektor Industri: ");
            gets(sekdus.sektorindustri);
            sekdus.sektorindustri[strcspn(sekdus.sektorindustri, "\n")] = '\0';
            if(strlen(sekdus.sektorindustri) == 0 || strlen(sekdus.sektorindustri) > 50) {
                printf("Nama Sektor Industri tidak valid! Harus 1-50 karakter.\n");
                return;
            }
            printf("Kata Sandi (8-10 karakter): ");
            gets(sekdus.katasandi);
            sekdus.katasandi[strcspn(sekdus.katasandi, "\n")] = '\0';
            if(strlen(sekdus.katasandi) < 8 || strlen(sekdus.katasandi) > 10) {
                printf("Kata Sandi tidak valid! Harus 8-10 karakter.\n");
                return;
            }
            sekdus.pemasukkan = malloc(sizeof(double) * maks_klien);
            sekdus.pengeluaran = malloc(sizeof(double) * maks_klien);
            sekdus.jumlahPemasukkan = 0;
            sekdus.jumlahPengeluaran = 0;
            sekdus.danaTotal = 0.0;
            listSekdus[jumlahklien++] = sekdus;

            printf("Registrasi berhasil!\n");
            printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
            getchar();
            SektorIndustri();
            break;
        case 2:
            LoginKlien();
            break;
        case 3:
            main();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            SektorIndustri();
            break;
    }
}

void LoginKlien() {
    char username[50], password[50];
    int i = 0;

    system("cls");
    printf("Masukkan Nama Pengguna: ");
    gets(username);
    printf("Masukkan password: ");
    gets(password);

    while(i < jumlahklien) {
        if(strcmp(listSekdus[i].username, username) == 0 && strcmp(listSekdus[i].katasandi, password) == 0) {
            printf("Login Berhasil!\n");
            getchar(); 
            MenuKlien(i);
            return;
        }
        i++;
    }
    printf("ERROR: Username atau password salah!\n");
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    SektorIndustri();
}

void MenuKlien(int i) {
    int pilihan;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: Klien :: Sektor Industri: %s, %s.\n\n", listSekdus[i].nama, listSekdus[i].sektorindustri);
    printf("Pilihan Menu:\n");
    printf("[1] Pendapatan Sektor Industri\n");
    printf("[2] Pengaturan Produksi\n");
    printf("[3] Pengaturan Kerjasama Dengan Kepala Daerah\n");
    printf("[4] Keluar\n");
    printf("Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    switch(pilihan) {
        case 1:
            AlurEkonomiSekdus(i);
            break;
        case 2:
            PengaturanProduksi(i);
            break;
        case 3:
            PengaturanKerjasamaKepda(i);
            break;
        case 4:
            SektorIndustri();
            break;
        default:
            printf("ERROR! Input yang diterima tidaklah valid!\n");
            printf("... Silahkan untuk dikondisikan kembali pilihan yang telah tersedia!\n");
            MenuKlien(i);
            break;
    }
}

void AlurEkonomiSekdus(int i) {
    system("cls");
    printf("\nBerikut adalah data akumulasi perekonomian dana industri dari yang bersangkutan:\n");
    printf("====================================================================================================\n");
    printf("Dana Perekonomian Total: Rp%.2f\n\n", listSekdus[i].danaTotal);
    printf("Pemasukkan (Incomes):\n");
    if (listSekdus[i].jumlahPemasukkan > 0) {
        for (int j = 0; j < listSekdus[i].jumlahPemasukkan; j++) {
            printf("  [%d] Rp%.2f\n", j + 1, listSekdus[i].pemasukkan[j]);
        }
    } else {
        printf("  Tidak ada pemasukan yang tercatat.\n");
    }
    printf("\nPengeluaran (Expenses):\n");
    if (listSekdus[i].jumlahPengeluaran > 0) {
        for (int j = 0; j < listSekdus[i].jumlahPengeluaran; j++) {
            printf("  [%d] Rp%.2f\n", j + 1, listSekdus[i].pengeluaran[j]);
        }
    } else {
        printf("  Tidak ada pengeluaran yang tercatat.\n");
    }
    printf("====================================================================================================\n");
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKlien(i);
}

void PengaturanProduksi(int i) {
    int pilihan;
    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: Client :: Sektor Industri: %s, %s.\n\n", listSekdus[i].nama, listSekdus[i].sektorindustri);
    printf("Menu: [2] Pengaturan Barang Produksi\n");
    printf("====================================================================================================\n");

    for (int j = 0; j < 5; j++) {
        if (listSekdus[i].stokProduk[j].jumlah > 0) {
            printf("[%d]: Nama Produk: %s\n", j + 1, listSekdus[i].stokProduk[j].namaProduk);
            printf("...  Stok Produk: %d\n", listSekdus[i].stokProduk[j].jumlah);
            printf("...  Harga Produk (per item): Rp%.2f\n\n", listSekdus[i].stokProduk[j].hargaPerItem);
        } else {
            printf("[%d]: Stok produk industri masih kosong\n", j + 1);
        }
    }
    printf("====================================================================================================\n");
    printf("Tersedia %d dari 5 stok produk industri seperti pada di atas.\n", listSekdus[i].jumlahProduk);
    printf("... TAMBAHKAN produk hasil industri baru?\n\n");
    printf("(ketik angka bebas dari [1] hingga [5] untuk dilakukan pengisian data produk baru)\n");
    printf("(jika tidak, silakan ketik [0] untuk kembali ke menu sebelumnya)\n");
    printf("> Pilihan Anda: ");
    scanf("%d", &pilihan);
    getchar();

    if (pilihan == 0) {
        MenuKlien(i);
    } else if (pilihan > 0 && pilihan <= 5) {
        int index = pilihan - 1;
        printf("\nMasukkan nama produk: ");
        fgets(listSekdus[i].stokProduk[index].namaProduk, 50, stdin);
        listSekdus[i].stokProduk[index].namaProduk[strcspn(listSekdus[i].stokProduk[index].namaProduk, "\n")] = '\0'; // Hapus newline
        printf("Masukkan jumlah produk: ");
        scanf("%d", &listSekdus[i].stokProduk[index].jumlah);
        printf("Masukkan harga per produk: ");
        scanf("%lf", &listSekdus[i].stokProduk[index].hargaPerItem);
        listSekdus[i].jumlahProduk++;
        printf("\nProduk berhasil ditambahkan!\n");
    } else {
        printf("Input tidak valid. Silakan coba lagi.\n");
    }
    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKlien(i);
}

void PengaturanKerjasamaKepda(int i) {
    int pilihanDaerah, jumlah;

    system("cls");
    printf("Selamat Datang di aplikasi: D'Wakanda!\n");
    printf("Anda tengah masuk sebagai: Client :: Sektor Industri: %s, %s.\n\n", 
           listSekdus[i].nama, listSekdus[i].sektorindustri);
    printf("Menu: [3] Pengaturan Kerjasama Dengan Kepala Daerah\n");
    printf("====================================================================================================\n");

    printf("Daftar Kepala Daerah:\n");
    for (int j = 0; j < jumlahpengguna; j++) { 
        printf("[%d] %s (%s)\n", j + 1, listKepda[j].nama, listKepda[j].daerah);
    }
    printf("[0] Kembali ke Menu Utama\n");
    printf("====================================================================================================\n");
    printf("> Pilih Kepala Daerah: ");
    scanf("%d", &pilihanDaerah);
    getchar();

    if (pilihanDaerah == 0) {
        MenuKlien(i); 
        return;
    }

    int indexDaerah = pilihanDaerah - 1;
    if (indexDaerah < 0 || indexDaerah >= jumlahpengguna) {
        printf("Pilihan tidak valid!\n");
        getchar();
        MenuKlien(i);
        return;
    }

    printf("\nProduk yang tersedia untuk pembelian:\n");
    for (int j = 0; j < 5; j++) {
        if (listSekdus[i].stokProduk[j].jumlah > 0) {
            printf("[%d] Nama Produk: %s\n", j + 1, listSekdus[i].stokProduk[j].namaProduk);
            printf("... Stok: %d, Harga: Rp%.2f\n", listSekdus[i].stokProduk[j].jumlah, listSekdus[i].stokProduk[j].hargaPerItem);
        }
    }
    printf("> Pilih produk yang akan dibeli: ");
    scanf("%d", &pilihanDaerah);
    printf("> Masukkan jumlah pembelian: ");
    scanf("%d", &jumlah);

    int indexProduk = pilihanDaerah - 1;
    if (jumlah <= listSekdus[i].stokProduk[indexProduk].jumlah) {
        double totalHarga = jumlah * listSekdus[i].stokProduk[indexProduk].hargaPerItem;
        listSekdus[i].stokProduk[indexProduk].jumlah -= jumlah;
        listSekdus[i].jumlahPemasukkan++;
        listSekdus[i].pemasukkan = realloc(listSekdus[i].pemasukkan, sizeof(double) * listSekdus[i].jumlahPemasukkan);
        listSekdus[i].pemasukkan[listSekdus[i].jumlahPemasukkan - 1] = totalHarga;
        listKepda[indexDaerah].belanjaProduk += totalHarga;

        printf("\nPembelian berhasil dicatat.\n");
    } else {
        printf("Stok tidak mencukupi!\n");
    }

    printf("\nTekan tombol [ENTER] untuk melanjutkan...\n");
    getchar();
    MenuKlien(i);
}

int validasiEmail(char *email) {
    return (strchr(email, '@') && strchr(email, '.'));
}

int validasiTanggal(char *tanggal) {
    if (strlen(tanggal) < 13) return 0;
    char *koma = strchr(tanggal, ',');
    if (koma == NULL || koma - tanggal < 1) return 0;
    char *bagian = koma + 2;
    if (strlen(bagian) != 10) return 0;
    if (bagian[2] != '/' || bagian[5] != '/') return 0;
    char hari[3], bulan[3], tahun[5];
    strncpy(hari, bagian, 2);
    hari[2] = '\0'; 
    strncpy(bulan, bagian + 3, 2);
    bulan[2] = '\0';
    strncpy(tahun, bagian + 6, 4);
    tahun[4] = '\0';

    int rubahhari = atoi(hari);
    int rubahbulan = atoi(bulan);
    int rubahtahun = atoi(tahun);

    if (rubahbulan < 1 || rubahbulan > 12) return 0;
    if (rubahhari < 1 || rubahhari > 31) return 0;
    if ((rubahbulan == 4 || rubahbulan == 6 || rubahbulan == 9 || rubahbulan == 11) && rubahbulan > 30) return 0;
    if (rubahbulan == 2) {
        if ((rubahtahun % 4 == 0 && rubahtahun % 100 != 0) || (rubahtahun % 400 == 0)) {
            if (rubahhari > 29) return 0;
        } else {
            if (rubahhari > 28) return 0;
        }
    }
    return 1;
}