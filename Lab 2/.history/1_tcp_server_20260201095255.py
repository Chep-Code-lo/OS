import socket
import math

def solve_quadratic(a, b, c):
    """Giải phương trình bậc 2: ax² + bx + c = 0"""
    try:
        if a == 0:
            if b == 0:
                if c == 0:
                    return "Phương trình vô số nghiệm"
                else:
                    return "Phương trình vô nghiệm"
            else:
                x = -c / b
                return f"Phương trình bậc nhất có nghiệm: x = {x:.4f}"
        
        # Tính delta
        delta = b**2 - 4*a*c
        
        if delta < 0:
            return "Phương trình vô nghiệm (delta < 0)"
        elif delta == 0:
            x = -b / (2*a)
            return f"Phương trình có nghiệm kép: x = {x:.4f}"
        else:
            x1 = (-b + math.sqrt(delta)) / (2*a)
            x2 = (-b - math.sqrt(delta)) / (2*a)
            return f"Phương trình có 2 nghiệm: x1 = {x1:.4f}, x2 = {x2:.4f}"
    except Exception as e:
        return f"Lỗi tính toán: {str(e)}"

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    HOST = '127.0.0.1'
    PORT = 5000
    server_socket.bind((HOST, PORT))

    server_socket.listen(5)
    print(f"TCP Server listening on {HOST}:{PORT}")
    print("Chờ client kết nối...\n")
    
    while True:
        client_socket, address = server_socket.accept()
        print(f"[+] Kết nối từ {address}")
        
        try:
            while True:
                # Nhận dữ liệu từ client
                data = client_socket.recv(1024).decode()
                
                if not data or data.strip().lower() == 'quit':
                    print(f"[-] Client {address} ngắt kết nối")
                    break
                
                print(f"Nhận từ client: {data}")
                
                # Parse 3 số a, b, c
                try:
                    numbers = data.split()
                    if len(numbers) != 3:
                        response = "Lỗi: Vui lòng gửi đúng 3 số a, b, c"
                    else:
                        a = float(numbers[0])
                        b = float(numbers[1])
                        c = float(numbers[2])
                        
                        print(f"Giải phương trình: {a}x² + {b}x + {c} = 0")
                        response = solve_quadratic(a, b, c)
                        print(f"Kết quả: {response}")
                except ValueError:
                    response = "Lỗi: Dữ liệu không hợp lệ. Vui lòng gửi 3 số"
                
                client_socket.send(response.encode())
            
        except Exception as e:
            print(f"[!] Lỗi kết nối: {str(e)}")
        
        finally:
            client_socket.close()
            print(f"Đã đóng kết nối với {address}\n")
            print("Chờ client mới...\n")

if __name__ == "__main__":
    main()
