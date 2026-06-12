#include "power_manager.hpp"

namespace {

// Rounded pill that slides on/off. Returns true
// when the value changed this frame.
bool ToggleSwitch(const char *id, bool *v) {
	ImVec2 pos = ImGui::GetCursorScreenPos();
	ImDrawList *draw = ImGui::GetWindowDrawList();

	float height = ImGui::GetFrameHeight() * 0.9f;
	float width = height * 1.8f;
	float radius = height * 0.5f;

	ImGui::InvisibleButton(id, ImVec2(width, height));
	bool changed = false;
	if (ImGui::IsItemClicked()) {
		*v = !*v;
		changed = true;
	}

	float t = *v ? 1.0f : 0.0f;
	ImU32 bgOn =
		ImGui::ColorConvertFloat4ToU32(ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
	ImU32 bgOff =
		ImGui::ColorConvertFloat4ToU32(ImVec4(0.35f, 0.35f, 0.35f, 1.0f));
	ImU32 bg = *v ? bgOn : bgOff;
	if (ImGui::IsItemHovered()) {
		bg = ImGui::ColorConvertFloat4ToU32(
			*v ? ImVec4(0.36f, 0.65f, 1.0f, 1.0f)
			   : ImVec4(0.45f, 0.45f, 0.45f, 1.0f));
	}

	draw->AddRectFilled(pos, ImVec2(pos.x + width, pos.y + height), bg, radius);
	float cx = pos.x + radius + t * (width - radius * 2.0f);
	draw->AddCircleFilled(ImVec2(cx, pos.y + radius), radius - 2.0f,
						  IM_COL32(255, 255, 255, 255));
	return changed;
}

// Label on the left, widget pushed to a fixed column on the right
void RowLabel(const char *label, float widgetColumn) {
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted(label);
	ImGui::SameLine();
	ImGui::SetCursorPosX(widgetColumn);
}

void SectionHeader(const char *text) {
	ImGui::Spacing();
	ImGui::TextUnformatted(text);
	ImGui::Spacing();
}

constexpr float kWidgetColumn = 280.0f;
constexpr float kComboWidth = 230.0f;

const char *kButtonActions[] = {"Do nothing", "Suspend",  "Hibernate",
								"Ask",		  "Shutdown", "Lock screen"};
const char *kSleepModes[] = {"Suspend", "Hibernate", "Hybrid sleep"};
const char *kPowerProfiles[] = {"power-saver", "balanced", "performance"};
const char *kCriticalActions[] = {"Notify", "Suspend", "Hibernate", "Shutdown"};

} // namespace

PowerManager::PowerManager()
	: Window("Power Manager",
			 ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings) {}

void PowerManager::drawPowerSourceSelector() {
	float total = 200.0f;
	float half = total * 0.5f;
	ImGui::SetCursorPosX((ImGui::GetWindowWidth() - total) * 0.5f);

	ImGui::PushStyleColor(ImGuiCol_Button,
						  m_pluggedIn ? ImVec4(0.30f, 0.30f, 0.30f, 1.0f)
									  : ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
	if (ImGui::Button("On battery", ImVec2(half, 0)))
		m_pluggedIn = false;
	ImGui::PopStyleColor();

	ImGui::SameLine(0.0f, 0.0f);

	ImGui::PushStyleColor(ImGuiCol_Button,
						  m_pluggedIn ? ImVec4(0.26f, 0.59f, 0.98f, 1.0f)
									  : ImVec4(0.30f, 0.30f, 0.30f, 1.0f));
	if (ImGui::Button("Plugged in", ImVec2(half, 0)))
		m_pluggedIn = true;
	ImGui::PopStyleColor();
}

void PowerManager::drawGeneralTab() {
	SectionHeader("Buttons");

	ImGui::Indent();
	ImGui::PushItemWidth(kComboWidth);

	RowLabel("When power button is pressed:", kWidgetColumn);
	ImGui::Combo("##powerbtn", &m_powerButtonAction, kButtonActions,
				 IM_ARRAYSIZE(kButtonActions));

	RowLabel("When sleep button is pressed:", kWidgetColumn);
	ImGui::Combo("##sleepbtn", &m_sleepButtonAction, kButtonActions,
				 IM_ARRAYSIZE(kButtonActions));

	RowLabel("When hibernate button is pressed:", kWidgetColumn);
	ImGui::Combo("##hibernatebtn", &m_hibernateButtonAction, kButtonActions,
				 IM_ARRAYSIZE(kButtonActions));

	RowLabel("When battery button is pressed:", kWidgetColumn);
	ImGui::Combo("##batterybtn", &m_batteryButtonAction, kButtonActions,
				 IM_ARRAYSIZE(kButtonActions));

	ImGui::PopItemWidth();

	RowLabel("Handle display brightness keys", kWidgetColumn);
	ToggleSwitch("##brightnesskeys", &m_handleBrightnessKeys);

	RowLabel("Brightness step count:", kWidgetColumn);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputInt("##brightnessstep", &m_brightnessStepCount, 1, 1);
	if (m_brightnessStepCount < 1)
		m_brightnessStepCount = 1;
	if (m_brightnessStepCount > 100)
		m_brightnessStepCount = 100;
	ImGui::SameLine();
	ImGui::Checkbox("Exponential", &m_brightnessExponential);

	ImGui::Unindent();

	SectionHeader("Appearance");

	ImGui::Indent();
	RowLabel("Status notifications", kWidgetColumn);
	ToggleSwitch("##statusnotif", &m_statusNotifications);

	RowLabel("System tray icon", kWidgetColumn);
	ToggleSwitch("##systray", &m_systemTrayIcon);
	ImGui::Unindent();
}

void PowerManager::drawSystemTab() {
	ImGui::Spacing();
	drawPowerSourceSelector();
	ImGui::Spacing();

	SectionHeader("System power saving");
	ImGui::Indent();
	ImGui::PushItemWidth(kComboWidth);

	RowLabel("System sleep mode:", kWidgetColumn);
	ImGui::Combo("##sleepmode", &m_systemSleepMode, kSleepModes,
				 IM_ARRAYSIZE(kSleepModes));

	RowLabel("When inactive for", kWidgetColumn);
	ImGui::SetNextItemWidth(kComboWidth);
	ImGui::SliderInt("##inactivefor", &m_inactiveFor, 0, 120,
					 m_inactiveFor == 0 ? "Never" : "%d min");
	ImGui::PopItemWidth();
	ImGui::Unindent();

	SectionHeader("Laptop Lid");
	ImGui::Indent();
	RowLabel("When laptop lid is closed:", kWidgetColumn);
	ImGui::SetNextItemWidth(kComboWidth);
	ImGui::Combo("##lidaction", &m_lidAction, kButtonActions,
				 IM_ARRAYSIZE(kButtonActions));
	ImGui::Unindent();

	SectionHeader("Power profile");
	ImGui::Indent();
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::Combo("##powerprofile", &m_powerProfile, kPowerProfiles,
				 IM_ARRAYSIZE(kPowerProfiles));
	ImGui::Unindent();

	SectionHeader("Critical power");
	ImGui::Indent();
	RowLabel("Critical battery power level:", kWidgetColumn);
	ImGui::SetNextItemWidth(70.0f);
	ImGui::InputInt("##criticallevel", &m_criticalLevel, 1, 1);
	if (m_criticalLevel < 1)
		m_criticalLevel = 1;
	if (m_criticalLevel > 20)
		m_criticalLevel = 20;
	ImGui::SameLine();
	ImGui::TextUnformatted("%");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(1.0f, 0.6f, 0.1f, 1.0f), "\xEF\x81\xB1");

	RowLabel("On critical battery power:", kWidgetColumn);
	ImGui::SetNextItemWidth(kComboWidth);
	ImGui::Combo("##criticalaction", &m_criticalAction, kCriticalActions,
				 IM_ARRAYSIZE(kCriticalActions));
	ImGui::Unindent();

	SectionHeader("Security");
	ImGui::Indent();
	ImGui::Checkbox("Lock screen when system is going to sleep",
					&m_lockOnSleep);
	ImGui::Unindent();
}

void PowerManager::drawDisplayTab() {
	ImGui::Spacing();
	drawPowerSourceSelector();
	ImGui::Spacing();

	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("Display power management");
	ImGui::SameLine();
	ImGui::SetCursorPosX(kWidgetColumn);
	ToggleSwitch("##dpm", &m_displayPowerManagement);
	ImGui::Spacing();

	ImGui::BeginDisabled(!m_displayPowerManagement);
	ImGui::Indent();
	RowLabel("Put to sleep after", kWidgetColumn);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::SliderInt("##sleepafter", &m_sleepAfter, 1, 60, "%d minutes");

	RowLabel("Switch off after", kWidgetColumn);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::SliderInt("##switchoff", &m_switchOffAfter, 1, 60, "%d minutes");
	ImGui::Unindent();
	ImGui::EndDisabled();

	SectionHeader("Brightness reduction");
	ImGui::Indent();
	RowLabel("On inactivity reduce to", kWidgetColumn);
	ImGui::SetNextItemWidth(-FLT_MIN);
	ImGui::SliderInt("##reduceto", &m_reduceBrightnessTo, 0, 100, "%d %%");

	RowLabel("Reduce after", kWidgetColumn);
	ImGui::SetNextItemWidth(-FLT_MIN);
	int mins = m_reduceAfter / 60;
	int secs = m_reduceAfter % 60;
	char buf[32];
	if (secs == 0)
		ImGui::SliderInt("##reduceafter", &m_reduceAfter, 30, 600, "%d sec");
	else {
		snprintf(buf, sizeof(buf), "%d minutes %d seconds", mins, secs);
		ImGui::SliderInt("##reduceafter", &m_reduceAfter, 30, 600, buf);
	}
	ImGui::Unindent();
}

void PowerManager::drawDevicesTab() {
	ImGui::Spacing();

	struct DeviceField {
		const char *key;
		const char *value;
	};

	// Two mock devices
	static const DeviceField kBatteryFields[] = {
		{"Device", "battery_BAT1"},
		{"Type", "Battery"},
		{"PowerSupply", "True"},
		{"Model", "AP18E7M"},
		{"Technology", "Lithium ion"},
		{"Current charge", "97%"},
		{"Fully charged (design)", "58.8 Wh"},
		{"Fully charged", "46.8 Wh (79%)"},
		{"Voltage", "17.1 V"},
		{"Vendor", "SMP"},
		{"Serial", "36865"},
	};
	static const DeviceField kLinePowerFields[] = {
		{"Device", "line_power_ACAD"},
		{"Type", "Line power"},
		{"PowerSupply", "True"},
	};

	const char *deviceNames[] = {"SMP AP18E7M", "Line power"};
	const char *deviceStatus[] = {"Fully charged", "Plugged in"};
	const char *deviceIcons[] = {"\xEF\x89\x80",
								 "\xEF\x87\xA6"}; // battery, plug

	// Left (device list)
	ImGui::BeginChild("##devicelist", ImVec2(170.0f, 0.0f), true);
	for (int i = 0; i < 2; ++i) {
		ImGui::PushID(i);
		bool selected = (m_selectedDevice == i);
		if (ImGui::Selectable("##devsel", selected, 0, ImVec2(0.0f, 44.0f)))
			m_selectedDevice = i;
		ImGui::SameLine(8.0f);
		ImGui::BeginGroup();
		ImGui::TextColored(ImVec4(0.4f, 0.85f, 0.4f, 1.0f), "%s",
						   deviceIcons[i]);
		ImGui::SameLine();
		ImGui::BeginGroup();
		ImGui::TextUnformatted(deviceNames[i]);
		ImGui::TextDisabled("%s", deviceStatus[i]);
		ImGui::EndGroup();
		ImGui::EndGroup();
		ImGui::PopID();
	}
	ImGui::EndChild();

	ImGui::SameLine();

	// Right (details for the selected device)
	ImGui::BeginChild("##devicedetails", ImVec2(0.0f, 0.0f), true);
	const DeviceField *fields =
		m_selectedDevice == 0 ? kBatteryFields : kLinePowerFields;
	int count = m_selectedDevice == 0 ? IM_ARRAYSIZE(kBatteryFields)
									  : IM_ARRAYSIZE(kLinePowerFields);
	if (ImGui::BeginTable("##details", 2, ImGuiTableFlags_SizingFixedFit)) {
		ImGui::TableSetupColumn("k", ImGuiTableColumnFlags_WidthFixed, 150.0f);
		ImGui::TableSetupColumn("v", ImGuiTableColumnFlags_WidthStretch);
		for (int i = 0; i < count; ++i) {
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			ImGui::TextUnformatted(fields[i].key);
			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(fields[i].value);
		}
		ImGui::EndTable();
	}
	ImGui::EndChild();
}

void PowerManager::draw() {
	if (!m_open)
		return;

	ImGui::SetNextWindowSize(size(), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(position(), ImGuiCond_FirstUseEver);

	bool open = m_open;
	if (!ImGui::Begin("Power Manager", &open,
					  ImGuiWindowFlags_NoCollapse |
						  ImGuiWindowFlags_NoSavedSettings)) {
		ImGui::End();
		m_open = open;
		return;
	}

	// Body area, leaving room for the bottom button bar
	float footer =
		ImGui::GetFrameHeightWithSpacing() + ImGui::GetStyle().ItemSpacing.y;
	ImGui::BeginChild("##body", ImVec2(0.0f, -footer));
	if (ImGui::BeginTabBar("##tabs")) {
		if (ImGui::BeginTabItem("General")) {
			drawGeneralTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("System")) {
			drawSystemTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Display")) {
			drawDisplayTab();
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Devices")) {
			drawDevicesTab();
			ImGui::EndTabItem();
		}
		ImGui::EndTabBar();
	}
	ImGui::EndChild();

	ImGui::Separator();
	ImGui::TextUnformatted("Design based from XFCE4 Power Manager");
	ImGui::SameLine();
	float closeWidth = 80.0f;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - closeWidth -
						 ImGui::GetStyle().WindowPadding.x);
	if (ImGui::Button("Close", ImVec2(closeWidth, 0)))
		open = false;

	ImGui::End();
	m_open = open;
}

ImVec2 PowerManager::position() {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImVec2 sz = size();
	return ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x - sz.x) * 0.5f,
				  viewport->WorkPos.y + (viewport->WorkSize.y - sz.y) * 0.5f);
}

ImVec2 PowerManager::size() { return ImVec2(560.0f, 580.0f); }
