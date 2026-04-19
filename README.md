# Module: System Calls Interface

## Tóm tắt công việc
Xây dựng cầu nối System Call giữa User Space và Kernel Space. Bổ sung và định tuyến các ngắt hệ thống theo thiết kế, đặc biệt xử lý ánh xạ (map) cho lệnh `memmap`.

## Các tệp và hàm cần hiện thực
* `src/syscall.c`: Hiện thực hàm `_syscall()`
* `src/sys_xxxhandler.c`: Hiện thực các System Call tương ứng
* `src/sys_mem.c`: Hiện thực hàm `__sys_memmap()`
* `src/syscalltbl.sh` và `src/syscall.tbl`: Đăng ký số hiệu cho syscall

## Nội dung đặc tả cần tham khảo
* Mục 1.6 (How to Write the Kernel Interface): Ứng dụng gọi thông qua thư viện `libstd`, tham số phải được đóng gói qua `struct sc_regs`.
* Mục 1.6.2 (The system call): Syscall 17 (`memmap`) phải được ánh xạ để gọi đúng các handler tương ứng: `SYSMEM_MAP_OP`, `SYSMEM_INC_OP`, `SYSMEM_SWP_OP`, `SYSMEM_IO_READ`, `SYSMEM_IO_WRITE`.

## Giấy phép
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
