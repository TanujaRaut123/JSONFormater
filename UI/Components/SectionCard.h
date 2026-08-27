#pragma once

#include <QFrame>

class QVBoxLayout;
class QString;

class SectionCard : public QFrame
{
public:
    explicit SectionCard(const QString &title, QWidget *parent = nullptr);

    QVBoxLayout *contentLayout() const;
};
