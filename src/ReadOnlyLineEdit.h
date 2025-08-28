#ifndef READONLYLINEEDIT_H
#define READONLYLINEEDIT_H

#include <QLineEdit>

/**
 * @brief 枠線付き読み取り専用ラインエディット (背景透過)。
 */
class ReadOnlyLineEdit : public QLineEdit {
public:
	/** @brief コンストラクタ。読み取り専用属性等を設定。 */
	ReadOnlyLineEdit(QWidget *parent = nullptr);
	/** @brief テキスト設定 (選択状態クリア)。 */
	void setText(QString const &text);
protected:
	/** @brief カスタム描画 (下線風ボーダー描画後に基底実装)。 */
	void paintEvent(QPaintEvent *event) override;
};

#endif // READONLYLINEEDIT_H
