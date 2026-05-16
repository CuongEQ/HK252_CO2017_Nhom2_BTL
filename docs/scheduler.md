# Module: Multi-Level Queue (MLQ) Scheduler

## Tóm tắt công việc
Hiện thực thuật toán lập lịch MLQ với nhiều hàng đợi ưu tiên. Đảm bảo CPU lấy tiến trình theo đúng mức độ ưu tiên và kiểm soát số lượng time slot được cấp phát.

## Các tệp và hàm cần hiện thực
* `src/queue.c`: Hiện thực hàm `enqueue()`, `dequeue()`
* `src/sched.c`: Hiện thực hàm `get_proc()`, `put_proc()`, `add_proc()`
* `include/os-cfg.h`: Kích hoạt lập lịch MLQ thông qua `#define MLQ_SCHED 1`

## Nội dung đặc tả cần tham khảo
* Mục 2.1 (Scheduler):
  * Nắm bắt luồng đi của tiến trình.
  * Sử dụng công thức tính `slot = MAX_PRIO - prio` với `MAX_PRIO = 140`.

**⚠️Lưu ý:** Bỏ qua hoàn toàn logic liên quan đến `run_queue` do tài liệu đã xác nhận đây là thiết kế lỗi thời.

## Giấy phép
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
