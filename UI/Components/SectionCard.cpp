#include "SectionCard.h"

#include <QLabel>
#include <QVBoxLayout>

SectionCard::SectionCard(const QString &title, QWidget *parent)
    : QFrame(parent)
{
    setObjectName(QStringLiteral("sectionCard"));
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 18, 20, 18);
    layout->setSpacing(10);

    auto *titleLabel = new QLabel(title, this);
    titleLabel->setObjectName(QStringLiteral("sectionCardTitle"));
    layout->addWidget(titleLabel);
}

QVBoxLayout *SectionCard::contentLayout() const
{
    return qobject_cast<QVBoxLayout *>(layout());
}
