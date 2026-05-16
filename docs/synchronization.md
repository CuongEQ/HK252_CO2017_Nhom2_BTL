# Module: Synchronization & Page Replacement

## Tóm tắt công việc
* Bảo vệ tính toàn vẹn của dữ liệu bằng cơ chế Mutex Lock khi nhiều CPU thao tác đồng thời.
* Đưa ra quyết định (Policy) để chọn trang bị thay thế (Victim Page) khi bộ nhớ vật lý đầy.

## Các tệp và hàm cần hiện thực
* `src/sched.c` và `src/queue.c`: Hiện thực khóa `pthread_mutex_lock()` và `pthread_mutex_unlock()`
* `src/mm-vm.c`: Hiện thực hàm `find_victim_page()`

## Nội dung đặc tả cần tham khảo
* Mục 2.3 (Put It All Together): Cần nhận diện các tài nguyên chia sẻ (hàng đợi `mlq_ready_queue`, cấu trúc `memphy_struct`) và đặt Lock trước khi đọc/ghi để tránh Race Condition.
* Mục 3.2 (Optional page replacement): Lựa chọn thuật toán thay thế trang (ví dụ FIFO). Duyệt qua danh sách `fifo_pgn` để trả về số hiệu trang cần đẩy đi.

**⚠️Lưu ý:** Tuyệt đối không thực hiện việc sao chép trang (Page Swapping) ở đây. Chỉ dừng lại ở thuật toán tìm trang bị thay thế (Victim Page).

## Giấy phép
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
