# Module: 64-bit Multi-level Paging

## Tóm tắt công việc
Xây dựng sơ đồ địa chỉ 64-bit với 5 mức phân trang (5-level paging). Tính toán và trích xuất chính xác các chỉ mục (index) từ địa chỉ ảo sang các cấp bảng trang tương ứng.

## Các tệp và hàm cần hiện thực
* `src/mm64.c`: Hiện thực hàm `get_pd_from_address()`, `get_pd_from_pagenum()`
* `include/mm64.h`: Định nghĩa các hằng số dịch bit (Shift/Mask)
* `include/os-cfg.h`: Kích hoạt 64-bit MLP thông qua `#define MM64 1`

## Nội dung đặc tả cần tham khảo
* Mục 2.2.2 (The virtual memory mapping in each process): Áp dụng sơ đồ Canonical của Linux. Cụ thể phân mức:
  * PGD (bit 48-56)
  * P4D (bit 39-47)
  * PUD (bit 30-38)
  * PMD (bit 21-29)
  * PT (bit 12-20).
* Mục 3.2 (Requirements - vmap_pgd_memset): Chuẩn bị cơ chế dummy allocation để mô phỏng hành vi của page-directory do không gian địa chỉ 64-bit rất lớn.

## Giấy phép
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
