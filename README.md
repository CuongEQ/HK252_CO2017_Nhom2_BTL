# Module: Multi-level Paging 64-bit

## Tóm tắt công việc
Hiện thực sơ đồ dịch địa chỉ 64-bit qua 5 cấp phân trang, tối ưu hóa kích thước bảng trang và hiện thực hàm giả lập cấp phát (`memset`).

## 1. Các tệp cần làm việc
* `src/mm64.c`: Xử lý toàn bộ logic phân trang 64-bit.
* Cần test bằng file cấu hình không có macro `#define MM_FIXED_MEMSZ`.

## 2. Công thức Dịch địa chỉ 5 cấp (Sử dụng Bitwise)
Địa chỉ 64-bit được chia thành các phần sau. Tại mỗi cấp, ta dịch phải (shift right) số bit tương ứng và AND với mask `0x1ff` (tương đương 9 bit) để lấy Index.

* **Level 5 (PGD - Page Global Directory):** Bit 56-48
  * `Index = (vaddr >> 48) & 0x1ff`
* **Level 4 (P4D - Page Level 4 Directory):** Bit 47-39
  * `Index = (vaddr >> 39) & 0x1ff`
* **Level 3 (PUD - Page Upper Directory):** Bit 38-30
  * `Index = (vaddr >> 30) & 0x1ff`
* **Level 2 (PMD - Page Middle Directory):** Bit 29-21
  * `Index = (vaddr >> 21) & 0x1ff`
* **Level 1 (PT - Page Table):** Bit 20-12
  * `Index = (vaddr >> 12) & 0x1ff`
* **Offset:** Bit 11-0
  * `Offset = vaddr & 0xfff` (Mặt nạ 12 bit)

## 3. Hướng dẫn từng bước
**Bước 1: Khai báo cấu trúc bảng đa cấp**
* Đảm bảo `mm_struct` sử dụng các con trỏ 64-bit (`uint64_t *pgd, *p4d, *pud, *pmd, *pt`) thay vì 32-bit.

**Bước 2: Viết hàm dịch địa chỉ**
* Viết hàm nhận vào địa chỉ ảo `vaddr`, sau đó dùng các công thức bitwise ở trên để bóc tách từng Index.
* Duyệt cây từ PGD -> P4D -> PUD -> PMD -> PT. Nếu một nhánh chưa tồn tại (Null), cần cấp phát khung trang mới cho nhánh đó (Sparse allocation).

**Bước 3: Viết hàm `vmap_pgd_memset`**
* Hệ thống 64-bit quá lớn không thể cấp phát RAM thật cho mọi mảng.
* Hàm này chỉ tạo lập các entry (đường dẫn) trong các Directory (từ PGD đến PT) mà không map tới một RAM vật lý nào. Chỉ đánh dấu là có ánh xạ, trả về thành công để pass test case cấu hình bộ nhớ khủng.
