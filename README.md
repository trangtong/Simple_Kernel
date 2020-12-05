# Simple_Kernel
Random a number and read it in user space

# Steps (open in terminal)
1. Chạy lệnh: make
2. Install module: sudo insmod Random.ko
3. Kiểm tra module cài đặt chưa: lsmod
4. Biên dịch file test.c: gcc test.c -o test
5. Chạy file test: sudo ./test
6. Kiểm tra số phát sinh: dmesg
7. Remove module: sudo rmmod Random.ko

# Explain code
static unsigned int randomResult;

static ssize_t my_read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
 printk(KERN_INFO "Driver: read()\n");
 
 get_random_bytes(&randomResult, sizeof(randomResult)); // Phát sinh số ngẫu nhiên có kích thước tương ứng với kích thước randomResult (sizeof(randomResult))
                                                        // và lưu vào randomResult
 // Copy kết quả vào buffer cho người dùng bằng hàm copy_to_user() 
 if (copy_to_user(buf, &randomResult, sizeof(randomResult))) { //Failed
  return -EFAULT;
  }
 return sizeof(randomResult);
}

# References
Tài liệu hướng dẫn của giảng viên
https://www.kernel.org/doc/htmldocs/kernel-api/API---copy-to-user.html
https://www.programmersought.com/article/4310539015/
https://stackoverflow.com/questions/40961482/how-to-use-get-random-bytes-in-linux-kernel-module
