# Module: Memory Management (Quản lý bộ nhớ)

## Tóm tắt công việc
Quản lý cấp phát/giải phóng vùng nhớ cho tiến trình và cài đặt cơ chế Swapping (hoán đổi trang giữa RAM và ổ cứng ảo).

## 1. Các tệp cần làm việc
* `src/mm.c`: Logic phân trang và quản lý vùng nhớ chính.
* `src/mm-vm.c`: Xử lý giới hạn vùng nhớ ảo.
* `src/mm-memphy.c`: Thao tác trực tiếp với RAM và SWAP.

## 2. Cấu trúc Page Table Entry (PTE)
* Dữ liệu 32-bit. Bạn cần dùng các phép bitwise (`&`, `|`, `>>`, `<<`) để thao tác:
  * Bit 31: Cờ Present (1 = có trên RAM, 0 = không có).
  * Bit 30: Cờ Swapped (1 = đang ở SWAP).
  * Bit 0-12: Chứa Page Frame Number (FPN) nếu đang trên RAM.
  * Bit 5-25: Chứa Swap Offset (Vị trí trên SWAP) nếu bị hoán đổi.

## 3. Luồng xử lý chi tiết (Trích từ Figure 7)
**ℹ️ Câu hỏi 1:** What is the primary motivation for combining segmentation with paging in memory manage
ment? How does this hybrid approach address limitations inherent in using either technique alone?

**ℹ️ Câu hỏi 2:** What are the benefits of extending hierarchical paging to N level?

**A. Lệnh `ALLOC` (Cấp phát)**
1. Tìm một vùng nhớ rỗng trong `vm_freerg_list` (Danh sách các vùng nhớ rỗng).
2. Nếu tìm thấy: Lấy vùng nhớ đó và trả về.
3. Nếu không tìm thấy (hết vùng rỗng):
   * Gọi system call `MEMINC` để nới rộng giới hạn bộ nhớ (`sbrk`).
   * Lấy vùng nhớ mới tạo ra ở giới hạn mới (`NEW LIMIT`).

**B. Lệnh `FREE` (Giải phóng)**
* Không xóa vật lý ngay. Chỉ cần lấy `vm_rg_struct` (Vùng nhớ rỗng) vừa được giải phóng và nối nó vào danh sách `vm_freerg_list` để tái sử dụng cho lệnh `ALLOC` sau này.

**C. Lệnh `READ` / `WRITE` và Swapping**
1. Lấy Page chứa địa chỉ cần đọc/ghi. Kiểm tra cờ `Present` (Thông qua bit 31).
2. **Nếu Page Present (Trang đang ở trên RAM):** Lấy Frame Number (FPN) và tiến hành truy xuất vật lý bằng `MEMIO`.
3. **Nếu Page NOT Present (Trang đang nằm dưới ổ SWAP):**
   * Phát sinh Page Fault. Gọi system call `MEMSWP`.
   * Tìm một Frame rỗng trên RAM. Nếu RAM đầy, tìm một Victim Page trên RAM và chuyển xuống SWAP (`SWAP COPY FROM RAM TO SWP`).
   * Nạp trang đang cần từ SWAP lên Frame rỗng vừa có trên RAM (`SWAP COPY FROM SWP TO RAM`).
   * Lấy FPN mới và thực hiện lệnh `READ`/`WRITE`.

**ℹ️ Câu hỏi 3:** What are the advantages and disadvantages of paging and contiguous memory allocation?

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
