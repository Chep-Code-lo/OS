import socket

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    HOST = '127.0.0.1'
    PORT = 5000
    
    try:
        client_socket.connect((HOST, PORT))
        print(f"[+] Đã kết nối với server {HOST}:{PORT}")
        print("Giải phương trình bậc 2: ax² + bx + c = 0")
        print("Nhập 3 số a, b, c (cách nhau bởi dấu cách)")
        print("Gõ 'quit' để thoát\n")
        
        while True:
            # Nhập từ người dùng
            message = input("Nhập a b c: ")
            
            # Kiểm tra nếu muốn thoát
            if message.strip().lower() == 'quit':
                client_socket.send('quit'.encode())
                print("Đang ngắt kết nối...")
                break
            
            # Gửi dữ liệu đến server
            client_socket.send(message.encode())
            
            # Nhận kết quả từ server
            response = client_socket.recv(1024).decode()
            print(f"Kết quả: {response}\n")
        
    except Exception as e:
        print(f"[!] Lỗi: {e}")
    
    finally:
        client_socket.close()
        print("\nĐã đóng kết nối")

if __name__ == "__main__":
    main()
