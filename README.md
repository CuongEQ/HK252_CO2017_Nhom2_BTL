# Bài tập lớn Hệ điều hành (CO2017) - HK252
Đây là Repo dùng để lưu mã nguồn và lịch sử commit phục vụ làm việc song song giữa các thành viên trong nhóm nhằm giải quyết BTL này

## Phân chia công việc
Mỗi thành viên được phân công 1 công việc tương ứng với 1 nhánh (branch) cụ thể như sau:
- Nguyễn Minh Cường (2410432): 
- Hân Trần ()
- Thảo Ly ()
- Trang ()
- Lê Minh Anh ()

## Cấu trúc của Repo
### Cấu trúc của mã nguồn
Mã nguồn của BTL được cấu trúc thành các nhóm tệp với chức năng riêng biệt như sau:

| Header Files | Source Files | Description |
| :--- | :--- | :--- |
| `timer.h` | `timer.c` | Defines/implements the system timer. |
| `cpu.h` | `cpu.c` | Virtual CPU implementation. |
| `queue.h` | `queue.c` | (Priority) queue operations for PCBs. |
| `sched.h` | `sched.c` | Scheduler functions. |
| `common.h` | `os.c` | Structs, functions, and main entry point. |
| `mm.h`, `mm64.h` | `mm.c`, `mm64.c` | Paging-based memory management. |
| `syscall.h` |`syscall.c` | System call headers and implementation. |

### Cấu trúc của các nhánh (Branch)
Repo này được phân chia thành nhiều nhánh (Branch) tương ứng với nhiều mục đích/nhiệm vụ khác nhau bao gồm:
- `main`: Nhánh chính, là sản phẩm cuối cùng đã được kiểm tra, dùng để nộp cho giảng viên.
- `development`: Nhánh phát triển, là nhánh nhận các yêu cầu gộp (Merge) của các thành viên, dùng để kiểm tra trước khi đẩy (Push) lên nhánh `master`.
- `feature/[name]`: Nhánh công việc, tương ứng với 1 công việc được phân công hiện thực. Miêu tả công việc xem chi tiết tệp `README.md` ở từng nhánh (Branch).
- `report`: Nhánh công việc, tương ứng với công việc soạn báo cáo LaTeX.

## Quy trình làm việc
### Khởi tạo nhánh lần đầu tiên
- Cấu hình git remote (GitHub): `git remote add origin https://github.com/CuongEQ/HK252_CO2017_Nhom2_BTL.git`
- Đồng bộ mã nguồn mới nhất từ nhánh `development`: `git checkout develop && git pull origin development`
- Tạo nhánh làm việc cá nhân: `git checkout -b feature/[name]`
- Hiện thực theo yêu cầu và tự kiểm thử.
- Chuyển toàn bộ nhánh cục bộ (Local) vào hàng chờ git: `git add.`
- Commit toàn bộ thay đổi với lời nhắn: `git commit -m "[Lời nhắn ở đây]"`
- Đẩy thay đổi lên nhánh remote (GitHub): `git push -u origin feature/[name]`

### Sau khi đã có nhánh cục bộ (Local)
**⚠️ Cảnh báo:** Với mỗi lần làm việc cần đồng bộ mã nguồn mới và hợp nhất (Merge). Không chịu trách nhiệm cho trường hợp hợp nhất lỗi do mã nguồn đã quá lỗi thời. 
- Đồng bộ mã nguồn mới nhất từ nhánh `development`: `git checkout develop && git pull origin development`
- Quay về nhánh làm việc cá nhân và hợp nhất 2 mã nguồn: `git checkout feature/[name] && git merge develop`
- Hiện thực theo yêu cầu và tự kiểm thử.
- Chuyển toàn bộ nhánh cục bộ (Local) vào hàng chờ git: `git add.`
- Commit toàn bộ thay đổi với lời nhắn: `git commit -m "[Lời nhắn ở đây]"`
- Đẩy thay đổi lên nhánh remote (GitHub): `git push origin feature/[name]`

Sau khi đã hoàn thành toàn bộ công việc được phân công, tiến hành vào Repo và tạo Pull Request (PR) từ nhánh làm việc sang nhánh `development` và chờ Nhóm trưởng duyệt nội dung.

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
