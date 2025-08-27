#include "ReadOnlyLineEdit.h"
#include "main.h"

#include <QPainter>

ReadOnlyLineEdit::ReadOnlyLineEdit(QWidget *parent)
	: QLineEdit(parent)
{
	setReadOnly(true);
	setFrame(false);
	setStyleSheet("* { background: transparent; }");
}

void ReadOnlyLineEdit::setText(QString const &text)
{
	ASSERT_MAIN_THREAD();
	
	QLineEdit::setText(text);
	setSelection(0, 0);
}

void ReadOnlyLineEdit::paintEvent(QPaintEvent *event)
{
	{
		auto panel_bg_color = palette().color(QPalette::Window);
		int r = panel_bg_color.red();
		int g = panel_bg_color.green();
		int b = panel_bg_color.blue();
		r = r * 4 / 5;
		g = g * 4 / 5;
		b = b * 4 / 5;

		QPainter pr(this);
		int y = height() - 1;
		pr.fillRect(0, y, width(), 1, QColor(r, g, b));
	}
	QLineEdit::paintEvent(event);
}
