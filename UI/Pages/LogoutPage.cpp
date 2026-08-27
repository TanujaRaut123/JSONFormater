#include "LogoutPage.h"

#include "../Components/SectionCard.h"

#include <QLabel>
#include <QVBoxLayout>

LogoutPage::LogoutPage(QWidget *parent)
    : QWidget(parent)
{
    setObjectName(QStringLiteral("logoutPage"));
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(30, 28, 30, 30);
    auto *card = new SectionCard(QStringLiteral("Logout"), this);
    card->contentLayout()->addWidget(new QLabel(QStringLiteral("Logout workspace ready"), card));
    card->contentLayout()->addStretch();
    layout->addWidget(card, 1);
}
