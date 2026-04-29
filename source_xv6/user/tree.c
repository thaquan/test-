#include "kernel/types.h"
#include "kernel/stat.h" // Chứa định nghĩa T_DIR (thư mục), T_FILE (tập tin)
#include "user/user.h"
#include "kernel/fs.h"   // Chứa cấu trúc dirent (directory entry - mục thư mục)

// Hàm đệ quy duyệt cây thư mục
void tree(char *path, int depth) {
  char buf[512], *p;
  int fd;
  struct dirent de; // Cấu trúc lưu tên và inode của 1 file/thư mục con
  struct stat st;   // Cấu trúc lưu thông tin chi tiết (nó là file hay thư mục)

  // 1. MỞ ĐƯỜNG DẪN
  // Dùng open() để mở. Nếu trả về < 0 nghĩa là mở thất bại (không tồn tại hoặc lỗi).
  if((fd = open(path, 0)) < 0){
    fprintf(2, "tree: cannot open %s\n", path);
    return;
  }

  // Lấy thông tin của đường dẫn vừa mở xem nó là gì.
  if(fstat(fd, &st) < 0){
    fprintf(2, "tree: cannot stat %s\n", path);
    close(fd);
    return;
  }

  // 2. ĐỌC NỘI DUNG BÊN TRONG THƯ MỤC
  // Vòng lặp này sẽ đọc từng "mục" (dirent) bên trong thư mục hiện tại.
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0) continue; // Bỏ qua các mục trống (đã bị xóa)
    
    // RẤT QUAN TRỌNG: Bỏ qua thư mục hiện tại "." và thư mục cha ".."
    // Nếu không có dòng này, chương trình sẽ tự gọi lại chính nó mãi mãi (lặp vô tận).
    if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
      continue;

    // 3. IN KHOẢNG TRẮNG ĐỂ THỤT LỀ
    // Dựa vào biến depth, mỗi cấp thụt vào 2 khoảng trắng.
    for(int i = 0; i < depth; i++) {
      printf("  "); 
    }

    // 4. XÂY DỰNG ĐƯỜNG DẪN MỚI
    // Ví dụ đang ở "a", thư mục con là "b" -> Phải ghép thành "a/b" để hệ thống hiểu.
    strcpy(buf, path);           // Chép "a" vào buf
    p = buf + strlen(buf);       // Con trỏ p nhảy đến cuối chữ "a"
    *p++ = '/';                  // Thêm dấu "/" thành "a/"
    memmove(p, de.name, DIRSIZ); // Thêm tên file/thư mục con vào thành "a/b"
    p[DIRSIZ] = 0;               // Chốt đuôi chuỗi bằng số 0

    // Lấy thông tin của cái "a/b" vừa tạo ra
    if(stat(buf, &st) < 0){
      printf("tree: cannot stat %s\n", buf);
      continue;
    }

    // 5. KIỂM TRA VÀ ĐỆ QUY
    if(st.type == T_DIR) {
      // Nếu nó là THƯ MỤC (T_DIR): In tên nó ra kèm dấu "/".
      printf("%s/\n", de.name);
      // GỌI ĐỆ QUY: Tự gọi lại hàm tree() để chui vào bên trong thư mục này, đồng thời tăng depth lên 1 cấp.
      tree(buf, depth + 1);
    } else {
      // Nếu nó là FILE bình thường: Chỉ in tên nó ra thôi, không đi sâu vào nữa.
      printf("%s\n", de.name);
    }
  }
  close(fd); // Đọc xong thư mục nào thì phải đóng thư mục đó lại để giải phóng bộ nhớ.
}

// 6. HÀM MAIN CHÍNH
int main(int argc, char *argv[]) {
  if(argc < 2){
    // In thư mục hiện tại ra trước, sau đó duyệt con từ độ sâu 1
    printf("./\n");
    tree(".", 1);
  } else {
    // In thư mục do người dùng nhập ra trước, sau đó duyệt con từ độ sâu 1
    printf("%s/\n", argv[1]);
    tree(argv[1], 1);
  }
  exit(0);
}
