# Module: System Calls & Dual-Mode Protection

## Tóm tắt công việc
Thêm các lệnh hệ thống (system calls) mới, cấu hình Makefile và thực thi quy tắc bảo vệ vùng nhớ của Kernel.

## 1. Các tệp cần làm việc
* `src/sys_xxxhandler.c`: Tệp tạo mới chứa logic của syscall.
* `src/syscall.tbl`: Tệp đăng ký và ánh xạ các syscall.
* `Makefile`: Đăng ký biên dịch tệp `.c` mới.
* `src/syscall.c`: Cập nhật logic điều hướng.

## 2. Quy tắc bảo vệ nghiêm ngặt (Dual-Mode)
* **KHÔNG ĐƯỢC PHÉP** lấy thông tin cấu trúc `pcb_t` bằng cách truyền trực tiếp pointer từ user mode.
* Phải lấy thông tin từ chế độ kernel: Dùng ID tiến trình (PID) tìm kiếm trong cấu trúc nhân `struct krnl_t` để móc ra `pcb_t` tương ứng.
* Sử dụng `copy_from_user` để đọc tham số từ user space và `copy_to_user` để đẩy kết quả trả về user space. Không được `read/write` trực tiếp địa chỉ.

## 3. Hướng dẫn thêm và đăng kí System Call

**Bước 1: Khởi tạo file Handler**
* Tạo file mới (vd: `src/sys_listsyscallhandler.c` hoặc `src/sys_memmaphandler.c`).
* Hiện thực hàm theo cấu trúc chuẩn: `int _sys_xxxhandler(struct pcb_t *caller, struct sc_regs* reg)`.
* Trích xuất tham số từ các thanh ghi (`reg->r1`, `reg->r2`...).
* Xử lý công việc (với `listsyscall`: in ra danh sách syscall. Với `memmap`: gọi hàm ánh xạ bộ nhớ).

**Bước 2: Đăng ký mã số trong bảng `syscall.tbl`**
* Mở `src/syscall.tbl`.
* Thêm một dòng mới theo cú pháp: `<Mã_số>  <Tên_gợi_nhớ>  <Tên_hàm_handler>`.
  * *Ví dụ:* `440  LISTSYS  sys_listsyscallhandler`

**Bước 3: Báo cho bộ biên dịch (Makefile)**
* Mở `Makefile`.
* Thêm file object vào biến `SYSCALL_OBJ`.
  * *Ví dụ:* `SYSCALL_OBJ += $(OBJ)/sys_listsyscallhandler.o`
* Chạy lệnh `make all` để biên dịch kernel mới.

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
