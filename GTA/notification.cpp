#include "notification.h"
#include "graphics.h"



void NotificationSystem::AddNotification(const std::string& title, const std::string& message, float duration) {
	if (notifications.size() >= 8) {// Limita o n�mero de notifica��es a 8
        notifications.erase(notifications.begin());
    }
    
    notifications.push_back({
        title,
        message,
        {0.1f, 0.1f, 0.1f, 0.9f}, // Background color
        {0.2f, 0.2f, 0.2f, 1.0f}, // Border color
        {1.0f, 1.0f, 1.0f, 1.0f}, // Text color
        duration,
        Sys_Milliseconds()
        });
}

void NotificationSystem::Render() {
    float notificationX = 20.0f;      // Posi��o X inicial
    float notificationY = 20.0f;      // Posi��o inicial Y
    float maxWidth = 400.0f;          // Largura m�xima do ret�ngulo

    float screenWidth = 1280.0f;      // Largura da tela
    float screenHeight = 1080.0f;     // Altura da tela

    int currentTime = Sys_Milliseconds();

    for (auto it = notifications.begin(); it != notifications.end();) {
        float elapsedTime = (currentTime - it->startTime) / 1000.0f; // Tempo decorrido em segundos

        // Controle de anima��o de crescimento
        float growDuration = 0.5f; // Dura��o da anima��o de crescimento (em segundos)
        float growProgress = (elapsedTime < growDuration) ? (elapsedTime / growDuration) : 1.0f; // Progresso de crescimento (de 0 a 1)

        if (elapsedTime >= it->duration / 1000.0f + growDuration) {
            // Remove notifica��es que expiraram ap�s o tempo de exibi��o
            it = notifications.erase(it);
            continue;
        }

        int titleLines = 1;
        for (char c : it->title) {
            if (c == '\n') titleLines++;
        }

        int messageLines = 1;
        for (char c : it->message) {
            if (c == '\n') messageLines++;
        }

        int totalLines = titleLines + messageLines;

        // Calcula o tamanho do texto
        float titleWidthRel = UI::Renderer::GetTextWidth(it->title.c_str(), 0, 0.4f, 0.4f); // Retorna entre 0.0 e 1.0
        float messageWidthRel = UI::Renderer::GetTextWidth(it->message.c_str(), 0, 0.3f, 0.3f); // Retorna entre 0.0 e 1.0

        float titleWidth = titleWidthRel * screenWidth; // Converte para pixels
        float messageWidth = messageWidthRel * screenWidth; // Converte para pixels

        float notificationWidth = std::max(titleWidth, messageWidth) + 20.0f; // Adiciona margem
        notificationWidth = std::min(notificationWidth, maxWidth);           // Limita ao m�ximo

        float lineHeight = 25.0f; // Altura de cada linha
        float notificationHeight = 17.0f + (totalLines * lineHeight); // Altura ajustada com base nas linhas

        // Ajusta o tamanho do ret�ngulo com base no progresso da anima��o de crescimento
        float animatedWidth = notificationWidth * growProgress;    // Largura animada
        float animatedHeight = notificationHeight * growProgress;  // Altura animada
        float animatedX = notificationX + (notificationWidth - animatedWidth) / 2.0f; // Centraliza horizontalmente
        float animatedY = notificationY + (notificationHeight - animatedHeight) / 2.0f; // Centraliza verticalmente

        // Desenha o fundo e a borda
        UI::Renderer::DrawBorderedRectangle(
            Rectangle(animatedX, animatedY, animatedWidth, animatedHeight),
            2.0f, // Tamanho da borda
            it->borderColor,
            it->backgroundColor,
            UI::AnchorPoint::TopLeft, 
            true // Desenha o fundo
        );

        // Apenas desenha o texto quando o ret�ngulo atinge o tamanho total
        if (growProgress >= 1.0f) {
            // Desenha o t�tulo
            UI::Renderer::DrawText(
                it->title,
                0, // Fonte
                Rectangle(notificationX + 10.0f, notificationY + 5, notificationWidth - 20.0f, totalLines * lineHeight),
                vec2_t{ 0.4f, 0.4f }, // Escala
                it->textColor, // Texto com opacidade total
                UI::TextAlign::Left,
                false
            );

            // Desenha a mensagem
            UI::Renderer::DrawText(
                it->message,
                0, // Fonte
                Rectangle(notificationX + 10.0f, notificationY + 10.0f + (titleLines * lineHeight), notificationWidth - 20.0f, messageLines * lineHeight),
                vec2_t{ 0.3f, 0.3f }, // Escala
                it->textColor, // Texto com opacidade total
                UI::TextAlign::Left,
                false
            );
        }

        notificationY += notificationHeight + 10.0f; // Atualiza a posi��o Y para a pr�xima notifica��o
        ++it;
    }
}



//void NotificationSystem::Render() {
//    float startX = -420.0f;           // Posi��o inicial fora da tela (esquerda)
//    float endX = 20.0f;               // Posi��o final onde a notifica��o ser� exibida
//    float notificationY = 20.0f;      // Posi��o inicial Y
//    float maxWidth = 400.0f;          // Largura m�xima do ret�ngulo
//
//    float screenWidth = 1280.0f;      // Largura da tela
//    float screenHeight = 1080.0f;     // Altura da tela
//
//    int currentTime = Sys_Milliseconds();
//
//    for (auto it = notifications.begin(); it != notifications.end();) {
//        float elapsedTime = (currentTime - it->startTime) / 1000.0f; // Tempo decorrido em segundos
//
//        // Controle de entrada e sa�da
//        float slideDuration = 0.5f; // Dura��o da anima��o de slide (em segundos)
//        float notificationX = endX; // Posi��o X padr�o (final)
//
//        if (elapsedTime < slideDuration) {
//            // Anima��o de entrada (move da esquerda para a posi��o final)
//            float progress = elapsedTime / slideDuration;
//            notificationX = startX + (endX - startX) * progress;
//        }
//        else if (elapsedTime >= it->duration / 1000.0f) {
//            // Anima��o de sa�da (move da posi��o final para fora da tela)
//            float fadeOutElapsed = elapsedTime - (it->duration / 1000.0f);
//            if (fadeOutElapsed < slideDuration) {
//                float progress = fadeOutElapsed / slideDuration;
//                notificationX = endX + (startX - endX) * progress;
//            }
//            else {
//                // Remove notifica��es que expiraram
//                it = notifications.erase(it);
//                continue;
//            }
//        }
//
//        // Garante que a notifica��o nunca ultrapasse os limites da tela
//        if (notificationX < startX) notificationX = startX;
//        if (notificationX > endX) notificationX = endX;
//
//        // Contar linhas no texto
//        int titleLines = 1;
//        for (char c : it->title) {
//            if (c == '\n') titleLines++;
//        }
//
//        int messageLines = 1;
//        for (char c : it->message) {
//            if (c == '\n') messageLines++;
//        }
//
//        int totalLines = titleLines + messageLines;
//
//        // Calcula o tamanho do texto
//        float titleWidthRel = UI::Renderer::GetTextWidth(it->title.c_str(), 0, 0.4f, 0.4f); // Retorna entre 0.0 e 1.0
//        float messageWidthRel = UI::Renderer::GetTextWidth(it->message.c_str(), 0, 0.3f, 0.3f); // Retorna entre 0.0 e 1.0
//
//        float titleWidth = titleWidthRel * screenWidth; // Converte para pixels
//        float messageWidth = messageWidthRel * screenWidth; // Converte para pixels
//
//        float notificationWidth = std::max(titleWidth, messageWidth) + 20.0f; // Adiciona margem
//        notificationWidth = std::min(notificationWidth, maxWidth);           // Limita ao m�ximo
//
//        float lineHeight = 25.0f; // Altura de cada linha
//        float notificationHeight = 17.0f + (totalLines * lineHeight); // Altura ajustada com base nas linhas
//
//        // Desenha o fundo e a borda
//        UI::Renderer::DrawBorderedRectangle(
//            Rectangle(notificationX, notificationY, notificationWidth, notificationHeight),
//            2.0f, // Tamanho da borda
//            it->borderColor,
//            it->backgroundColor,
//            UI::AnchorPoint::TopLeft,
//            true // Desenha o fundo
//        );
//
//        // Desenha o t�tulo
//        UI::Renderer::DrawText(
//            it->title,
//            0, // Fonte
//            Rectangle(notificationX + 10.0f, notificationY + 5, notificationWidth - 20.0f, totalLines * lineHeight),
//            vec2_t{ 0.4f, 0.4f }, // Escala
//            it->textColor, // Texto com opacidade total
//            UI::TextAlign::Left,
//            false
//        );
//
//        // Desenha a mensagem
//        UI::Renderer::DrawText(
//            it->message,
//            0, // Fonte
//            Rectangle(notificationX + 10.0f, notificationY + 10.0f + (titleLines * lineHeight), notificationWidth - 20.0f, messageLines * lineHeight),
//            vec2_t{ 0.3f, 0.3f }, // Escala
//            it->textColor, // Texto com opacidade total
//            UI::TextAlign::Left,
//            false
//        );
//
//        notificationY += notificationHeight + 10.0f; // Atualiza a posi��o Y para a pr�xima notifica��o
//        ++it;
//    }
//}

NotificationSystem notificationSystem;