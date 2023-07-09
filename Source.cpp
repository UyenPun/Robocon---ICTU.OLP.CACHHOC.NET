#include <iostream>
#include <fstream>
using namespace std;

#define MAX 100

struct Robot
{
	int dong, cot; // vị trí Robot di chuyển trên map
	char huongmat; // hướng mặt Robot đang hướng về hướng nào trên màn hình (W: Trên, S: Dưới, A: Trái, D: Phải) để từ đó biết được Robot đi thẳng sẽ đi về hướng nào, rẽ trái mặt sẽ hướng về hướng nào, rẻ phải mặt sẽ hướng về hướng nào
	int lotrinh[MAX]; // Lộ trình đi của Robot
	bool truyvet[MAX][MAX]; // Nhờ vào mảng này đối chiếu vị trí tọa độ của map sang mảng truyvet này để biết được nước đi đó Robot đã từng đi hay chưa?
	int diemso; // Điểm số của Robot
};
typedef struct Robot ROBOT;

void KhoiTaoThongTinRobot(ROBOT &robot, int n, int dong_start, int cot_start, char huongmat_start, int diem_start)
{
	robot.dong = dong_start;
	robot.cot = cot_start;
	robot.huongmat = huongmat_start;
	robot.diemso = diem_start;

	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < n; ++j)
		{
			robot.truyvet[i][j] = false; // Khởi tạo tất cả ban đầu là false => nếu nước nào đi sau này thì đánh dấu là true. Thì nhờ đó mỗi lần cần kiểm tra 1 nước đi tại vị trí x, y tương ứng có được đi chưa chỉ cần đối chiếu vào mảng 2 chiều truy vết là biết ngay (false: chưa đi, true: đã đi)
		}
	}
}

// lenh có thể là 0 hay 1 hay 2. Với lệnh = 1 hay 2 thì chúng ta xét lại hướng mặt cho con robot, nếu lệnh = 0 thì cập nhật lại vị trí tọa độ dòng - cột của Robot
void RobotNhanLenh(ROBOT &robot, int lenh, int map[MAX][MAX])
{
	if(lenh == 1) // đứng yên nhưng quay sang trái
	{
		if(robot.huongmat == 'W')
			robot.huongmat = 'A';
		else if(robot.huongmat == 'S')
			robot.huongmat = 'D';
		else if(robot.huongmat == 'A')
			robot.huongmat = 'S';
		else if(robot.huongmat == 'D')
			robot.huongmat = 'W';
	}
	else if(lenh == 2) // đứng yên nhưng quay sang phải
	{
		if(robot.huongmat == 'W')
			robot.huongmat = 'D';
		else if(robot.huongmat == 'S')
			robot.huongmat = 'A';
		else if(robot.huongmat == 'A')
			robot.huongmat = 'W';
		else if(robot.huongmat == 'D')
			robot.huongmat = 'S';
	}
	else if(lenh == 0) // tiến tới 1 bước => tùy vào mặt nó đang hướng về hướng nào của màn hình mà sẽ có cách di chuyển khác
	{
		if(robot.huongmat == 'W')
			robot.dong--;
		else if(robot.huongmat == 'S')
			robot.dong++;
		else if(robot.huongmat == 'A')
			robot.cot--;
		else if(robot.huongmat == 'D')
			robot.cot++;

		if(robot.truyvet[robot.dong][robot.cot] == false) // nước này chưa được đi
		{
			robot.diemso += map[robot.dong][robot.cot]; // cộng điểm số tương ứng bên map
			robot.truyvet[robot.dong][robot.cot] == true; // đã đi nước này
		}
	}
}

// Sẽ tiến hành cho 2 con Robot di chuyển theo lộ trình mà nó được cài đặt sẵn và cho đến khi 1 trong 2 điều kiện dừng xảy ra
// 1: 2 con đụng nhau khi chưa kết thúc lộ trình
// 2: 2 con đi hết lộ trình
void RobotDiChuyenTheoLoTrinh(int map[MAX][MAX], int m, ROBOT &robot_xanh, ROBOT &robot_do)
{
	// Vòng lặp xét qua danh sách lộ trình của mỗi Robot
	for(int i = 0; i < m; ++i)
	{
		RobotNhanLenh(robot_xanh, robot_xanh.lotrinh[i], map);
		RobotNhanLenh(robot_do, robot_do.lotrinh[i], map);

		if(robot_xanh.dong == robot_do.dong && robot_xanh.cot == robot_do.cot)
		{
			// Do không tính điểm tại ô bị đụng nhau nên phải trừ đi
			robot_xanh.diemso -= map[robot_xanh.dong][robot_xanh.cot];
			robot_do.diemso -= map[robot_do.dong][robot_do.cot];

			break; // Đụng nhau => thoát ra không xét lộ trình nữa
		}
	}
}

int main()
{
	ifstream FileIn;
	FileIn.open("input.txt", ios_base::in);

	int t; // số lượng test case
	FileIn >> t;
	
	// vòng lặp chạy với mỗi test case tương ứng để đọc dữ liệu từ tập tin vào chương trình
	for(int k = 1; k <= t; ++k)
	{
		int n, m;
		int map[MAX][MAX];

		FileIn >> n >> m;
		
		for(int i = 0; i < n; ++i)
		{
			for(int j = 0; j < n; ++j)
			{
				FileIn >> map[i][j];
			}
		}

		ROBOT robot_xanh, robot_do;

		for(int i = 0; i < m; ++i)
			FileIn >> robot_xanh.lotrinh[i];

		for(int i = 0; i < m; ++i)
			FileIn >> robot_do.lotrinh[i];

		// Bắt đầu xử lý từ chỗ này trở xuống

		// Bước 1: Khởi tạo thông tin ban đầu cho mỗi Robot
		KhoiTaoThongTinRobot(robot_xanh, n, 0, 0, 'S', 0);
		KhoiTaoThongTinRobot(robot_do, n, n - 1, n - 1, 'W', 0);

		// Bước 2: Cho Robot di chuyển theo lộ trình
		RobotDiChuyenTheoLoTrinh(map, m, robot_xanh, robot_do);

		// Xuất ra điểm số tương ứng của Robot xanh & Robot đỏ
		cout << robot_xanh.diemso << " " << robot_do.diemso << endl;

	}

	FileIn.close();

	system("pause");
	return 0;
}