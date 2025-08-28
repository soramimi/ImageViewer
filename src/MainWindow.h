#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class Video;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 * @brief 画像ビューアのメインウィンドウクラス。
 *
 * メニュー操作 (ファイルを開く / 設定 / 画像フィット / コピー) と
 * 画像表示ウィジェット `ImageWidget` を管理します。
 */
class MainWindow : public QMainWindow {
	Q_OBJECT
	friend class ImageWidget;
	friend class InternalImageView;
	friend class ThumbnailCache;
private:
	Ui::MainWindow *ui;
    struct Private;
	Private *m;
	/**
	 * @brief ファイルパスから画像を読み込み表示する内部関数。
	 * @param path 読み込む画像ファイルのパス
	 * @return 成功した場合 true / 失敗した場合 false
	 */
	bool openFile(const QString &path);
protected:
	/**
	 * @brief ウィンドウクローズ時のイベント処理。
	 * 設定で有効な場合、ウィンドウ位置・サイズを保存します。
	 */
	void closeEvent(QCloseEvent *event) override;
public:
	/**
	 * @brief コンストラクタ。UI初期化と前回状態の復元を行います。
	 */
	MainWindow(QWidget *parent = nullptr);
	/**
	 * @brief デストラクタ。
	 */
	~MainWindow();
	/**
	 * @brief 指定インデックスのサムネイル画像を取得 (未実装: たたき台)。
	 * @param index インデックス
	 * @param add キャッシュへ追加するかどうか
	 * @return サムネイル画像 (存在しない場合は null QImage)
	 */
	QImage thumbnail(int64_t index, bool add);
private slots:
	/** @brief [ファイル]→[開く] アクションハンドラ。 */
	void on_action_file_open_triggered();
	/** @brief [画像をビューにフィット] チェック変更時のハンドラ。 */
	void on_action_fit_image_to_view_changed();
	/** @brief 現在表示中の画像をクリップボードへコピー。 */
	void on_action_copy_triggered();
	/** @brief 設定ダイアログ表示ハンドラ。 */
	void on_action_settings_triggered();

	// QWidget interface
};
#endif // MAINWINDOW_H
