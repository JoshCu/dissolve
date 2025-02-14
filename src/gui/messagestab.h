// SPDX-License-Identifier: GPL-3.0-or-later
// Copyright (c) 2021 Team Dissolve and contributors

#pragma once

#include "gui/maintab.h"
#include "gui/outputhandler.hui"
#include "gui/ui_messagestab.h"

// Messages Tab
class MessagesTab : public QWidget, public MainTab
{
    // All Qt declarations derived from QObject must include this macro
    Q_OBJECT

    public:
    MessagesTab(DissolveWindow *dissolveWindow, Dissolve &dissolve, MainTabsWidget *parent, const QString title);
    ~MessagesTab() = default;

    /*
     * UI
     */
    private:
    Ui::MessagesTab ui_;
    // Output handler for messaging in GUI
    GUIOutputHandler outputHandler_;

    public slots:
    void on_MessagesIncreaseFontSizeButton_clicked(bool checked);
    void on_MessagesDecreaseFontSizeButton_clicked(bool checked);
    void clearMessages();
    void appendMessage(const QString &msg);

    /*
     * MainTab Reimplementations
     */
    public:
    // Return tab type
    MainTab::TabType type() const;

    /*
     * Update
     */
    protected:
    // Update controls in tab
    void updateControls();
    // Disable sensitive controls within tab
    void disableSensitiveControls();
    // Enable sensitive controls within tab
    void enableSensitiveControls();
};
