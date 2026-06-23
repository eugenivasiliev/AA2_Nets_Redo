#pragma once
#include "Scene.h"
#include "Button.h"
#include "NetworkManager.h"
#include "LoginStyle.h"
#include "InputField.h"
#include "ObjectShape.h"
#include "ObjectText.h"
#include "PlaceholderText.h"
#include "StatusText.h"

class LoginScene : public Scene
{
	sf::Font* font = nullptr;

	InputField* nicknameField = nullptr;
	InputField* passwordField = nullptr;

	std::string statusMessage = "Enter your credentials";
	sf::Color statusColor = statusNeutralColor;

public:
	void attemptLogin() {
		if (!nicknameField || !passwordField)
			return;

		if (nicknameField->value.empty() || passwordField->value.empty()) {
			statusMessage = "Please fill in nickname and password";
			statusColor = statusErrorColor;
			return;
		}

		NT->SendLogin(nicknameField->value, passwordField->value);
		statusMessage = "Trying to log in...";
		statusColor = statusNeutralColor;
	}

	void attemptRegister() {
		if (!nicknameField || !passwordField)
			return;

		if (nicknameField->value.empty() || passwordField->value.empty()) {
			statusMessage = "Please fill in nickname and password";
			statusColor = statusErrorColor;
			return;
		}

		NT->SendRegister(nicknameField->value, passwordField->value);
		statusMessage = "Trying to register...";
		statusColor = statusNeutralColor;
	}

	void enter(SharedMemory* _sharedMemory) override {
		sharedMemory = _sharedMemory;
		font = new sf::Font(FONT_PATH);

		// BACKGROUND
		sf::RectangleShape background;
		background.setSize({ WINDOW_WIDTH, WINDOW_HEIGHT });
		background.setPosition({ 0.f, 0.f });
		background.setFillColor(loginBackgroundColor);
		objects.push_back(new ObjectShape(background));

		// PANEL SHADOW
		sf::RectangleShape panelShadow;
		panelShadow.setSize(loginPanelSize);
		panelShadow.setPosition({
			WINDOW_WIDTH / 2.f - loginPanelSize.x / 2.f + 8.f,
			WINDOW_HEIGHT / 2.f - loginPanelSize.y / 2.f + 10.f
			});
		panelShadow.setFillColor(sf::Color(0, 0, 0, 100));
		objects.push_back(new ObjectShape(panelShadow));

		// PANEL
		sf::RectangleShape panel;
		panel.setSize(loginPanelSize);
		panel.setPosition({
			WINDOW_WIDTH / 2.f - loginPanelSize.x / 2.f,
			WINDOW_HEIGHT / 2.f - loginPanelSize.y / 2.f
			});
		panel.setFillColor(loginPanelColor);
		panel.setOutlineThickness(loginPanelOutlineThickness);
		panel.setOutlineColor(loginPanelOutlineColor);
		objects.push_back(new ObjectShape(panel));

		// ACCENT LINE
		sf::RectangleShape accentLine;
		accentLine.setSize({ loginPanelSize.x, 6.f });
		accentLine.setPosition(panel.getPosition());
		accentLine.setFillColor(panelAccentColor);
		objects.push_back(new ObjectShape(accentLine));

		// TITLE
		sf::Text title(*font, loginTitleText);
		title.setCharacterSize(loginTitleSize);
		title.setFillColor(loginTitleColor);
		{
			sf::FloatRect bounds = title.getLocalBounds();
			title.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.14f
				});
		}
		objects.push_back(new ObjectText(title));

		// SUBTITLE
		sf::Text subtitle(*font, loginSubtitleText);
		subtitle.setCharacterSize(loginSubtitleSize);
		subtitle.setFillColor(loginSubtitleColor);
		{
			sf::FloatRect bounds = subtitle.getLocalBounds();
			subtitle.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.205f
				});
		}
		objects.push_back(new ObjectText(subtitle));

		// NICKNAME BOX
		sf::RectangleShape nicknameRect;
		nicknameRect.setSize(nicknameRectSize);
		nicknameRect.setPosition({
			WINDOW_WIDTH * nicknameRectAnchorX - nicknameRect.getSize().x / 2.f,
			WINDOW_HEIGHT * nicknameRectAnchorY
			});
		nicknameRect.setFillColor(nicknameRectColour);
		nicknameRect.setOutlineThickness(inputOutlineThickness);
		nicknameRect.setOutlineColor(inputIdleOutlineColor);

		sf::Text nicknameLabel(*font, "Nickname");
		nicknameLabel.setCharacterSize(inputLabelSize);
		nicknameLabel.setFillColor(inputLabelColor);
		nicknameLabel.setPosition({
			nicknameRect.getPosition().x,
			nicknameRect.getPosition().y - 28.f
			});
		objects.push_back(new ObjectText(nicknameLabel));

		nicknameField = new InputField(nicknameRect, sf::Text(*font), nicknameMaxChars);
		nicknameField->setTextCharacterSize(inputTextSize);
		nicknameField->setTextColor(inputTextColor);
		nicknameField->setIdleOutlineColor(inputIdleOutlineColor);
		nicknameField->setFocusOutlineColor(inputFocusOutlineColor);
		objects.push_back(nicknameField);

		sf::Text nicknamePlaceholder(*font, "Enter your nickname");
		nicknamePlaceholder.setCharacterSize(inputPlaceholderSize);
		nicknamePlaceholder.setFillColor(inputPlaceholderColor);
		nicknamePlaceholder.setPosition({
			nicknameRect.getPosition().x + 14.f,
			nicknameRect.getPosition().y + nicknameRect.getSize().y / 2.f - 15.f
			});
		objects.push_back(new PlaceholderText(nicknamePlaceholder, nicknameField));

		// PASSWORD BOX
		sf::RectangleShape passwordRect;
		passwordRect.setSize(passwordRectSize);
		passwordRect.setPosition({
			WINDOW_WIDTH * passwordRectAnchorX - passwordRect.getSize().x / 2.f,
			WINDOW_HEIGHT * passwordRectAnchorY
			});
		passwordRect.setFillColor(passwordRectColour);
		passwordRect.setOutlineThickness(inputOutlineThickness);
		passwordRect.setOutlineColor(inputIdleOutlineColor);

		sf::Text passwordLabel(*font, "Password");
		passwordLabel.setCharacterSize(inputLabelSize);
		passwordLabel.setFillColor(inputLabelColor);
		passwordLabel.setPosition({
			passwordRect.getPosition().x,
			passwordRect.getPosition().y - 28.f
			});
		objects.push_back(new ObjectText(passwordLabel));

		passwordField = new InputField(passwordRect, sf::Text(*font), passwordMaxChars);
		passwordField->isCensored = true;
		passwordField->setTextCharacterSize(inputTextSize);
		passwordField->setTextColor(inputTextColor);
		passwordField->setIdleOutlineColor(inputIdleOutlineColor);
		passwordField->setFocusOutlineColor(inputFocusOutlineColor);
		objects.push_back(passwordField);

		sf::Text passwordPlaceholder(*font, "Enter your password");
		passwordPlaceholder.setCharacterSize(inputPlaceholderSize);
		passwordPlaceholder.setFillColor(inputPlaceholderColor);
		passwordPlaceholder.setPosition({
			passwordRect.getPosition().x + 14.f,
			passwordRect.getPosition().y + passwordRect.getSize().y / 2.f - 15.f
			});
		objects.push_back(new PlaceholderText(passwordPlaceholder, passwordField));

		// LOGIN BUTTON
		sf::RectangleShape loginRect;
		loginRect.setSize(loginRectSize);
		loginRect.setPosition({
			WINDOW_WIDTH * loginRectAnchorX - loginRect.getSize().x / 2.f,
			WINDOW_HEIGHT * loginRectAnchorY
			});
		loginRect.setFillColor(loginRectColour);

		sf::Text loginText(*font, loginButtonLabel);
		loginText.setCharacterSize(buttonTextSize);
		loginText.setFillColor(sf::Color::White);

		Button* loginButton = new Button(loginRect, loginText, [this]() {
			attemptLogin();
			});
		objects.push_back(loginButton);

		// REGISTER BUTTON
		sf::RectangleShape registerRect;
		registerRect.setSize(registerRectSize);
		registerRect.setPosition({
			WINDOW_WIDTH * registerRectAnchorX - registerRect.getSize().x / 2.f,
			WINDOW_HEIGHT * registerRectAnchorY
			});
		registerRect.setFillColor(registerRectColour);

		sf::Text registerText(*font, registerButtonLabel);
		registerText.setCharacterSize(buttonTextSize);
		registerText.setFillColor(sf::Color::White);

		Button* registerButton = new Button(registerRect, registerText, [this]() {
			attemptRegister();
			});
		objects.push_back(registerButton);

		// STATUS TEXT
		sf::Text statusBase(*font, "");
		statusBase.setCharacterSize(statusTextSize);
		statusBase.setFillColor(statusNeutralColor);
		statusBase.setPosition({ 0.f, WINDOW_HEIGHT * 0.84f });
		objects.push_back(new StatusText(statusBase, &statusMessage, &statusColor));

		// FOOTER
		sf::Text footer(*font, loginFooterText);
		footer.setCharacterSize(loginFooterSize);
		footer.setFillColor(loginFooterColor);
		{
			sf::FloatRect bounds = footer.getLocalBounds();
			footer.setPosition({
				WINDOW_WIDTH / 2.f - bounds.size.x / 2.f - bounds.position.x,
				WINDOW_HEIGHT * 0.89f
				});
		}
		objects.push_back(new ObjectText(footer));
	}

	bool update(sf::RenderWindow& window) override
	{
		if (NT->GetLoginStatus()) {
			NT->SetNickname(nicknameField->value);
			statusMessage = "Login successful";
			statusColor = statusSuccessColor;
			nextScene = SceneOption::LOBBY;
		}

		return Scene::update(window);
	}

	void handleEvent(const sf::Event& event)
	{

		if (const sf::Event::KeyPressed* keyPressed = event.getIf<sf::Event::KeyPressed>()) {
			if (keyPressed->code == sf::Keyboard::Key::Enter) {
				attemptLogin();
			}
			else if (keyPressed->code == sf::Keyboard::Key::Tab && nicknameField && passwordField) {
				if (nicknameField->isFocused) {
					nicknameField->setFocused(false);
					passwordField->setFocused(true);
				}
				else {
					passwordField->setFocused(false);
					nicknameField->setFocused(true);
				}
			}
		}
	}
};