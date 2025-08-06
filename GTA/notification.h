#pragma once
#include "stdafx.h"


struct Notification {
    std::string title;
    std::string message;
    vec4_t backgroundColor;
    vec4_t borderColor;
    vec4_t textColor;
    float duration; // Dura��o em segundos
    int startTime;  // Tempo de in�cio em milissegundos
};

class NotificationSystem {
private:
    std::vector<Notification> notifications;

public:
    // Adiciona uma nova notifica��o
    void AddNotification(const std::string& title, const std::string& message, float duration = 5000.0f);

    // Renderiza as notifica��es
    void Render();
};

extern NotificationSystem notificationSystem;