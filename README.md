# Laporan Tugas Kecil 1 IF4031 - Pengembangan Aplikasi Terdistribusi

#### Devin Alvaro / 13515062

#### Rizki Ihza Parama / 13515104

Nomor 1:

- Tools benchmark yang digunakan: ApacheBench

- Cara melakukan tes:

  a. NGINX:

  1. Menjalankan NGINX untuk men-serve file-file html di direktori `./1-measure/templates`.
  2. Menjalankan ApacheBench dengan command `ab -n 10000 -c 10000 http://localhost:8081/<file>` di mana file-file yang dimaksud adalah `small.html` (500 bytes) dan `big.html` (20 KB).

  Kedua langkah dijalankan dengan Docker, karena saat menjalankan langsung di host, kami menemui limitasi konkurensi.
  Penggunaan Docker ini terdapat di script `./1-measure/nginx-docker.sh` dan `./1-measure/ab-docker.sh`.

  b. Apache:

  1. Menjalankan Apache dengan cara `./apache-docker.sh` untuk men-serve file-file html di direktori `./1-measure/templates`
  2. Menjalankan ApacheBench dengan command `ab -n 10000 -c 10000 http://localhost:8080/<file>` di mana file-file yang dimaksud adalah `small.html` (500 bytes) dan `big.html` (20 KB).

* Hasil:

  a. NGINX:

  - rata-rata waktu respons per request:
    1.  500 bytes: 0.221 ms
    2.  20 KB : 0.2717 ms
  - total waktu respons per request:
    1.  500 bytes: 2212.680 ms
    2.  20 KB : 2170.092 ms
  - memori yang digunakan:
    1.  500 bytes: ~1.3 MiB
    2.  20 KB : ~1.8 MiB

  b. Apache:

  - rata-rata waktu respons per request:
    1.  500 bytes: 2.505 ms
    2.  20 KB : 6.442 ms
  - total waktu respons per request:
    1.  500 bytes: 25050.011 ms
    2.  20 KB : 64415.433 ms
  - memori yang digunakan:
    1.  500 bytes: ~20 MiB
    2.  20 KB : ~34 MiB

Nomor 2:

- Petunjuk instalasi:

  1. `cd 2-serve`
  2. `make` untuk menjalankan server langsung di host atau `./docker.sh` untuk menjalankan server dengan Docker.
  3. Server menerima request di `http://localhost:8080/small/` (500 bytes) dan `http://localhost:8080/big/` (20 KB).

- Hasil:
  - rata-rata waktu respons per request:
    1. 500 bytes: 0.242 ms
    2. 20 KB : 0.445 ms
  - total waktu respons per request:
    1. 500 bytes: 2417.571 ms
    2. 20 KB : 4454.174 ms
  - memori yang digunakan:
    1. 500 bytes: ~1.5 MiB
    2. 20 KB : ~2.1 MiB
