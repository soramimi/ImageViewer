#ifndef APPLICATIONSETTINGS_H
#define APPLICATIONSETTINGS_H

#include <QColor>
#include <QString>

#define ORGANIZATION_NAME "soramimi.jp"
#define APPLICATION_NAME "ImageViewer"

/**
 * @brief アプリケーションの静的パス・名称等の基本データ保持。
 */
class ApplicationBasicData {
public:
	QString organization_name = ORGANIZATION_NAME; //!< 組織名
	QString application_name = APPLICATION_NAME;   //!< アプリ名
	QString this_executive_program;                //!< 実行ファイルパス
	QString generic_config_dir;                    //!< 共通設定ディレクトリ
	QString app_config_dir;                        //!< アプリ固有設定ディレクトリ
	QString log_dir;                               //!< ログディレクトリ
	QString config_file_path;                      //!< 設定ファイルパス
};

/**
 * @brief 保存/ロード可能なアプリ設定項目。
 */
class ApplicationSettings {
public:
	bool remember_and_restore_window_position = false; //!< ウィンドウ位置復元

	/** @brief 設定ファイルより読み込み。 */
	static ApplicationSettings loadSettings();
	/** @brief 現在値を設定ファイルへ保存。 */
	void saveSettings() const;

	/** @brief デフォルト値生成。 */
	static ApplicationSettings defaultSettings();

#if 0
	static QString loadOpenAiApiKey();
	static void saveOpenAiApiKey(const QString &key);
#endif
};

#endif // APPLICATIONSETTINGS_H
