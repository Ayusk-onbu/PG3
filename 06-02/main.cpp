#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>

struct MapRow {
	int y;
	std::vector<int> rowData;
};

void DrawMap(const std::vector<std::vector<int>>& mapData) {
	std::cout << "--- Map Display ---\n";

	for (const auto& row : mapData) {
		for (size_t i = 0; i < row.size(); ++i) {
			std::cout << row[i] << (i == row.size() - 1 ? "" : ",");
		}
		std::cout << "\n";
	}
	std::cout << "-------------------\n";
}

int main(){

	std::mutex mutex;
	std::queue<MapRow> finishedRows;
	bool isFinished = false;

	// バックグラウンドCSV読み込み
	std::thread loader([&]() {
		std::ifstream file("map.csv");
		std::string line;
		int rowIndex = 0;

		while (std::getline(file, line)) {
			MapRow row;
			row.y = rowIndex++;

			std::stringstream ss(line);
			std::string value;
			while (std::getline(ss, value, ',')) {
				row.rowData.push_back(std::stoi(value));
			}

			// キューにデータを保護して入れる
			{
				std::lock_guard<std::mutex> lock(mutex);
				finishedRows.push(row);
			}
		}
		isFinished = true; // 全ての読み込み完了
	});

	std::vector<std::vector<int>> mapData; // 最終的なマップ格納用

	// メインループ
	while (true) {
		// キューにデータがあれば取り出す
		{
			std::lock_guard<std::mutex> lock(mutex);
			while (!finishedRows.empty()) {
				MapRow data = finishedRows.front();
				finishedRows.pop();

				// ここで実際のマップデータ配列に反映させる
				if (mapData.size() <= data.y) mapData.resize(data.y + 1);
				mapData[data.y] = data.rowData;
			}
		}

		// 全読み込み完了かつキューが空ならループ終了
		if (isFinished && finishedRows.empty()) {
			// 描画処理
			DrawMap(mapData);
			break;
		}

	}

	loader.join();
	return 0;
}