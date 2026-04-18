# Module: Scheduler (Bộ định thời CPU)

## Tóm tắt công việc
Quản lý việc lập lịch tiến trình bằng thuật toán **Multi-Level Queue (MLQ)** và đảm bảo an toàn luồng (synchronization) trên môi trường đa lõi (Multi-CPU).

## 1. Các tệp cần làm việc
* `src/queue.c`: Xử lý hàng đợi ưu tiên.
* `src/sched.c`: Xử lý logic chọn tiến trình cho CPU.

## 2. Thông số kỹ thuật MLQ (Phải tuân thủ)
* **Số lượng hàng đợi ưu tiên:** `MAX_PRIO = 140` (từ mức 0 đến 139). Ưu tiên cao nhất là `prio = 0`.
* **Công thức Slot (Khe thời gian):** Mỗi hàng đợi có một số lượng slot cố định để sử dụng CPU trước khi bị bắt buộc chuyển sang hàng đợi tiếp theo.
  * `slot = MAX_PRIO - prio`
  * *Ví dụ:* Queue `prio = 0` có 140 slots. Queue `prio = 139` có 1 slot.
* **Quy tắc duyệt hàng đợi:** Phải duyệt qua danh sách các hàng đợi ưu tiên theo số bước/slot cố định này. Khi một hàng đợi xài hết slot, hệ thống chuyển sang hàng đợi của mức ưu tiên tiếp theo dù hàng đợi cũ vẫn còn tiến trình.

## 3. Hướng dẫn từng bước (Step-by-step)

**Bước 1: Hoàn thiện `queue.c`**
* Viết hàm `enqueue()`: Chèn một PCB (`struct pcb_t`) vào đúng vị trí cuối của `queue_t` được chỉ định. Đảm bảo cập nhật kích thước hàng đợi.
* Viết hàm `dequeue()`: Lấy PCB ở đầu hàng đợi ra ngoài và trả về con trỏ đến PCB đó.

**Bước 2: Hoàn thiện `sched.c`**
* Tìm hàm `get_proc()`: Viết logic lấy tiến trình tiếp theo.
* Duyệt mảng `mlq_ready_queue`. Kiểm tra xem hàng đợi hiện tại còn tiến trình và còn slot hay không.
* Nếu còn, gọi `dequeue()` và trừ đi 1 slot của hàng đợi đó.
* Nếu hết slot hoặc hết tiến trình, reset lại số slot của hàng đợi đó và nhảy sang hàng đợi ưu tiên thấp hơn tiếp theo.

**ℹ️ Trả lời câu hỏi:** Considering the impactness of MLQ detailed policies, what is the benefit of each policy?

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
