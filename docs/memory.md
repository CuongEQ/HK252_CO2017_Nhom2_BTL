# Module: Core Paging & Virtual Memory

## Tóm tắt công việc
Xử lý cấp phát, giải phóng và truy xuất bộ nhớ ảo dựa trên cơ chế phân trang (Paging). Thực hiện thao tác chuyển đổi trang (Swapping) thực tế giữa MRAM và MSWP khi có yêu cầu.

## Các tệp và hàm cần hiện thực
* `src/mm-vm.c`: Hiện thực hàm `get_free_vmrg_area()`, `inc_vma_limit()`
* `src/libmem.c`: Hiện thực hàm `pg_getpage()`, `__alloc()`, `__free()`, `__read()`, `__write()`

## Nội dung đặc tả cần tham khảo
* Mục 2.2.4 (Paging-based address translation scheme): Xử lý cơ chế Page Fault. Khi trang không nằm trong RAM (Present bit = 0), gọi hàm `find_victim_page()` để lấy trang cần xóa, sau đó dùng `__swap_cp_page()` để hoán đổi.
* Mục 3.2 (Requirements): Tuyệt đối không truy cập trực tiếp bộ nhớ bằng con trỏ `pcb_t` từ user space, phải lấy dữ liệu thông qua cấu trúc `krnl_t`.

**⚠️Lưu ý:** Không hiện thực hàm `find_victim_page()` mà chỉ thực hiện gọi đến hàm này khi cần thiết.

## Giấy phép
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
