# Bài tập lớn Hệ điều hành (CO2017) - HK252
Đây là Repo dùng để lưu mã nguồn và lịch sử commit phục vụ làm việc song song giữa các thành viên trong nhóm nhằm giải quyết BTL này

## Phân chia công việc
Mỗi thành viên được phân công 1 công việc tương ứng với 1 nhánh (branch) cụ thể như sau:
- Nguyễn Minh Cường (2410432): `feature/syscall`, tham khảo đặc tả [tại đây](docs/syscall.md)
- Trần Gia Hân (2052464): `feature/paging`, tham khảo đặc tả [tại đây](docs/paging.md)
- Phạm Nguyễn Thảo Ly (2411995): `feature/synchronization`, tham khảo đặc tả [tại đây](docs/synchronization.md)
- Lưu Bảo Trang (2413556): `feature/scheduler`, tham khảo đặc tả [tại đây](docs/scheduler.md)
- Lê Minh Anh (2410089): `feature/memory` tham khảo đặc tả [tại đây](docs/memory.md)

Ngoài ra còn có một số nhóm tệp khác có chức năng như sau:
- `docs.md`: Đặc tả của BTL này, được viết lại dưới dạng Markdown để dễ sử dụng trong các công cụ AI
- `Makefile`: Tệp Makefile dùng để biên dịch dự án tự động
- `docs/*.md`: Đặc tả công việc của từng nhánh

### Cấu trúc của các nhánh (Branch)
Repo này được phân chia thành nhiều nhánh (Branch) tương ứng với nhiều mục đích/nhiệm vụ khác nhau bao gồm:
- `master`: Nhánh chính, là sản phẩm cuối cùng đã được kiểm tra, dùng để nộp cho giảng viên.
- `development`: Nhánh phát triển, là nhánh nhận các yêu cầu gộp (Merge) của các thành viên, dùng để kiểm tra trước khi đẩy (Push) lên nhánh `master`.
- `feature/[name]`: Nhánh công việc, tương ứng với 1 công việc được phân công hiện thực. Miêu tả công việc xem chi tiết tệp `README.md` ở từng nhánh (Branch).
- `report`: Nhánh công việc, tương ứng với công việc soạn báo cáo LaTeX.

## Quy trình làm việc (Workflow)
### Khởi tạo nhánh lần đầu tiên
- Cấu hình git ưu tiên Merge thay vì Rebase: `git config --global pull.rebase false`
- Cấu hình git remote (GitHub): `git remote add origin https://github.com/CuongEQ/HK252_CO2017_Nhom2_BTL.git`
- Đồng bộ mã nguồn mới nhất từ nhánh `development`: `git checkout -b develop && git pull origin development`
- Tạo nhánh làm việc cá nhân: `git checkout -b feature/[name]`
- Hiện thực theo yêu cầu và tự kiểm thử.
- Chuyển toàn bộ nhánh cục bộ (Local) vào hàng chờ git: `git add.`
- Commit toàn bộ thay đổi với lời nhắn: `git commit -m "[Lời nhắn ở đây]"`
- Đẩy thay đổi lên nhánh remote (GitHub): `git push -u origin feature/[name]`

### Sau khi đã có nhánh cục bộ (Local)
**⚠️ Cảnh báo:** Với mỗi lần làm việc cần đồng bộ mã nguồn mới và hợp nhất (Merge). Không chịu trách nhiệm cho trường hợp hợp nhất lỗi do mã nguồn đã quá lỗi thời. 
- Đồng bộ mã nguồn mới nhất từ nhánh `development`: `git checkout feature/[name] && git merge development`
- Hiện thực theo yêu cầu và tự kiểm thử.
- Chuyển toàn bộ nhánh cục bộ (Local) vào hàng chờ git: `git add.`
- Commit toàn bộ thay đổi với lời nhắn: `git commit -m "[Lời nhắn ở đây]"`
- Đẩy thay đổi lên nhánh remote (GitHub): `git push origin feature/[name]`

Sau khi đã hoàn thành toàn bộ công việc được phân công, tiến hành vào Repo và tạo Pull Request (PR) từ nhánh làm việc sang nhánh `development` và chờ Nhóm trưởng duyệt nội dung.

**⚠️ Cảnh báo:** NGHIÊM CẤM việc sử dụng lệnh `git force push` để ép hợp nhất mã nguồn khi bị lỗi.

### Các công việc trong báo cáo LaTeX
Mỗi thành viên trả lời toàn bộ các câu hỏi và hoàn thành các yêu cầu cụ thể trong phần nội dung được phân công tại [đây](https://docs.google.com/document/d/1S8fYgDP6bGCTXm3XdoTbrI4CBD_PQ478BppXsM5i3kQ/edit?usp=sharing).

## Cách biên dịch, chạy và dọn dẹp mã nguồn
- Biên dịch toàn bộ dự án: `make all`
- Khởi động hệ điều hành: `./os [os_config]`. Trong đó `[os_config]` là tên của cấu hình đầu vào đặt ở thu mục `input` của gốc dự án
- Dọn dẹp dự án: `make clean`

## Giấy phép (License)
Khung mã nguồn mẫu thuộc bản quyền của các giảng viên có tham gia hiện thực thuộc trường Đại học Bách Khoa TPHCM (HCMUT) - Khoa Khoa học và Kĩ thuật máy tính (CSE). Giấy phép được cấp cho các sinh viên đang học môn Hệ điều hành (CO2017) với mục đích học tập.
