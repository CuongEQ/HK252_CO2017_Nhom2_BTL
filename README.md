# Module: Synchronization (Đồng bộ hoá)

## Trách nhiệm chính
Đảm bảo an toàn luồng (synchronization) trên môi trường đa lõi (Multi-CPU).

## 1. Các tệp cần làm việc
Toàn bộ các tệp có hàm hiện thực yêu cầu làm việc song song.

## 2. Hướng dẫn từng bước (Step-by-step)
* Tìm các biến được dùng chung khi khởi chạy CPU. Một số VD bao gồm:
  * Bộ lập lịch: Cấu trúc `ready_queue` (đặc biệt khi nhiều CPU ảo gọi hàm `get_proc()` cùng một lúc để lấy tiến trình).
  * Bộ nhớ vật lý: Các danh sách quản lý khung nhớ như `free_fp_list` và `used_fp_list` trong thao tác cấp phát/thu hồi frame.
  * Bộ nhớ thứ cấp: Cấu trúc quản lý ổ mram và các ổ mswp khi xảy ra hiện tượng hoán đổi (swapping). 
* Sử dụng khóa (`pthread_mutex_t` hoặc cơ chế tương đương trong base code) khóa lại trước khi `enqueue()`, `dequeue()` hoặc `get_proc()` truy xuất vào hàng đợi để tránh xung đột dữ liệu giữa các CPU.

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
