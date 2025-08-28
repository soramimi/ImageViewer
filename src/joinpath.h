#ifndef __JOINPATH_H
#define __JOINPATH_H

#include <string>

/** @brief 2つのパス文字列を結合 (末尾/先頭区切り除去後に '/' で連結)。 */
std::string joinpath(char const *left, char const *right);
/** @brief 文字列版 joinpath の std::string オーバーロード。 */
std::string joinpath(std::string const &left, std::string const &right);

static inline std::string operator / (std::string const &left, std::string const &right)
{
	return joinpath(left, right);
}

#include <QString>
/** @brief QString 版 joinpath 実体。 */
QString qjoinpath(ushort const *left, ushort const *right);
/** @brief QString 版 joinpath ラッパ。 */
inline QString joinpath(QString const &left, QString const &right)
{
	return qjoinpath(left.utf16(), right.utf16());
}

static inline QString operator / (QString const &left, QString const &right)
{
	return joinpath(left, right);
}

#endif
