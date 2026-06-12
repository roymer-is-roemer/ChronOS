#include "mail.hpp"
#include <cstring>

namespace {

// --- Nerd Font glyphs ---
constexpr const char *kIconGetMail = "";	 // download
constexpr const char *kIconSend = "";	 // paper-plane
constexpr const char *kIconCompose = "";	 // pencil
constexpr const char *kIconReply = "";	 // reply
constexpr const char *kIconReplyAll = ""; // reply-all
constexpr const char *kIconSender = "";	 // user
constexpr const char *kIconForward = "";	 // forward/share
constexpr const char *kIconTrash = "";	 // trash
constexpr const char *kIconSpam = "";	 // ban

constexpr const char *kIconMailbox = ""; // envelope
constexpr const char *kIconInbox = "";	// inbox
constexpr const char *kIconSent = "";	// paper-plane
constexpr const char *kIconDrafts = "";	// pencil-square
constexpr const char *kIconQueue = "";	// clock
constexpr const char *kIconFolderTrash = "";

constexpr const char *kIconSearch = "";		  // magnifying glass
constexpr const char *kIconPaperclip = "";	  // paperclip
constexpr const char *kIconCaret = "";		  // caret-down
constexpr const char *kIconEnvelopeOpen = ""; // read
constexpr const char *kIconEnvelope = "";	  // unread
constexpr const char *kIconOnline = "";		  // bolt (connection)

struct Folder {
	const char *icon;
	const char *name;
	int newCount;
	int unread;
	int total;
	int indent;
};

const Folder kFolders[] = {
	{kIconMailbox, "Mailbox (MH)", 0, 0, 0, 0},
	{kIconInbox, "Inbox", 0, 2, 4, 1},
	{kIconSent, "Sent", 0, 0, 0, 1},
	{kIconDrafts, "Drafts", 0, 0, 0, 1},
	{kIconQueue, "Queue", 0, 0, 0, 1},
	{kIconFolderTrash, "Trash", 0, 0, 0, 1},
};

struct Message {
	const char *subject;
	const char *from; // display name shown in the list
	const char *date; // short date shown in the list
	const char *size;
	bool unread;
	const char *fromAddr; // full From: header in the preview
	const char *to;		  // To: header in the preview
	const char *dateFull; // Date: header in the preview
	const char *body;	  // message body shown in the preview
};

const Message kMessages[] = {
	{"Friendly Reminder: Your Library Items Are Due Soon!", "library",
	 "06/07/2026(Sun) 3:01", "2.79KB", false, "library <library@dlsu.edu.ph>",
	 "Justin Rainier Go <justin_rainier_go@dlsu.edu.ph>",
	 "Sun, 7 Jun 2026 03:01:23 +0000",
	 "Courtesy Due Notice -- 06/07/2026\n\n"
	 "Dear JUSTIN RAINIER GO:\n\n"
	 "Good day! This is just a friendly reminder that the item/s you borrowed\n"
	 "from the library is due for return soon. Here are the details:\n\n"
	 "| Title                     | Author                 | Due Date   | "
	 "Library              |\n"
	 "| ------------------------- | ---------------------- | ---------- | "
	 "-------------------- |\n"
	 "| Operating system concepts | Silberschatz, Abraham. | 06/09/2026 | The "
	 "Learning Commons |\n\n"
	 "We encourage you to return the item/s on or before the due date to\n"
	 "avoid any late fees and to ensure they remain available for other\n"
	 "library users.\n\n"
	 "Need More Time?\n\n"
	 "You can renew your items easily by logging into your MyLibrary\n"
	 "account: https://animosearch.dlsu.edu.ph\n"},

	{"ACM TechNews, Wednesday, June 10, 2026", "ACM TechNews",
	 "06/10/2026(Wed) 11:40 PM", "64.2KB", false,
	 "ACM TechNews <technews-editor@acm.org>",
	 "Justin Rainier A Go <justin_rainier_go@dlsu.edu.ph>",
	 "Wed, 10 Jun 2026 23:40:00 +0000",
	 "Welcome to the June 10, 2026 edition of ACM TechNews, providing timely\n"
	 "information for computer professionals three times a week.\n\n"
	 "U.K. Commits £750M to National AI Supercomputer in £1.1B Hardware "
	 "Strategy\n"
	 "The U.K. unveiled a £1.1 billion (U.S.$1.48 billion) AI Hardware Plan "
	 "to\n"
	 "strengthen domestic AI and semiconductor capabilities, including £750\n"
	 "million (U.S.$1 billion) for a national AI supercomputer expected by\n"
	 "2030. The strategy supports British chip companies through funding,\n"
	 "procurement opportunities, and a new AI Hardware Innovation Program,\n"
	 "while expanding skills training for engineers and chip designers.\n"
	 "  HPCwire (June 8, 2026)\n\n"
	 "Sea Drone Rescues Crew of U.S. Apache Helicopter Shot Down by Iran\n"
	 "Two pilots aboard a U.S. Army AH-64 Apache helicopter operating near "
	 "the\n"
	 "Strait of Hormuz that was shot down by Iran Monday survived without\n"
	 "injury and were rescued in what officials described as the first-ever\n"
	 "military water rescue conducted using an unmanned surface drone.\n"
	 "  CBS News; Charlie D'Agata; Eleanor Watson; Frank Andrews (June 9, "
	 "2026)\n\n"
	 "Bot Web Traffic Overtakes Human Web Traffic\n"
	 "According to data from Internet hosting service provider Cloudflare,\n"
	 "automated bots and AI agents now generate more Web traffic than humans,\n"
	 "accounting for 57.4% of Internet requests. Cloudflare CEO Matthew "
	 "Prince\n"
	 "said the shift happened much sooner than expected, driven by rapid\n"
	 "growth in AI agents that can autonomously browse thousands of websites.\n"
	 "  NBC News; Samantha Elkins (June 4, 2026)\n\n"
	 "Pentagon Expands List of China Military-Linked Firms\n"
	 "The Pentagon expanded its list of companies allegedly linked to China's\n"
	 "military, adding major firms including Alibaba Group, Baidu, carmaker\n"
	 "BYD, biotech giant WuXi AppTec, and Unitree, a maker of humanoid "
	 "robots.\n"
	 "While the designation does not impose sanctions, it bars the U.S.\n"
	 "Defense Department from contracting with listed firms directly.\n"
	 "  CNBC; Anniek Bao (June 9, 2026)\n\n"
	 "Anthropic Releases 'Safe' Version of Claude Mythos AI Model to Public\n"
	 "Anthropic has released Fable 5, a public version of its advanced Mythos\n"
	 "AI model, while keeping the full-powered Mythos 5 model restricted to\n"
	 "approved organizations due to cybersecurity concerns. Fable 5 is\n"
	 "designed for tasks such as coding, research, and image analysis, but\n"
	 "sensitive requests are automatically routed to a lower-tier model.\n"
	 "  The Guardian (U.K.) (June 9, 2026)\n\n"
	 "Tech Jobs Grew in May Despite AI Layoffs\n"
	 "The U.S. tech job market continued to grow in May despite a surge in\n"
	 "AI-related layoffs. Employers added 69,000 technology jobs, according "
	 "to\n"
	 "a CompTIA review of U.S. Bureau of Labor Statistics data, while\n"
	 "unemployment among tech workers fell to 3.1% from 3.5% in April.\n"
	 "  CIO Dive; Roberto Torres (June 5, 2026)\n\n"
	 "Federal Judge Strikes Down $100,000 Fee on New H-1B Visas\n"
	 "A federal judge in Massachusetts blocked the Trump administration's\n"
	 "$100,000 fee on new H-1B visa applications, ruling that the executive\n"
	 "branch exceeded its authority by effectively imposing a tax without\n"
	 "congressional approval. The White House vowed to appeal the decision.\n"
	 "  Associated Press; Michael Casey (June 8, 2026)\n\n"
	 "Meta Launches $115M Skilled Trades Academy\n"
	 "Meta is launching America's Workforce Academy, a $115 million "
	 "initiative\n"
	 "that will provide free skilled-trades training and guaranteed jobs for\n"
	 "graduates. The program will begin with pilot locations in four states "
	 "--\n"
	 "Louisiana, Ohio, Indiana, and Texas.\n"
	 "  Fox Business; Eric Revell (June 8, 2026)\n\n"
	 "PepsiCo Delivering Doritos, Soft Drinks to Stores Using Driverless "
	 "Trucks\n"
	 "PepsiCo has deployed 35 fully driverless trucks in Arizona, plus\n"
	 "additional vehicles in Texas and Arkansas, becoming the first major "
	 "U.S.\n"
	 "consumer-goods company to operate autonomous delivery trucks at scale,\n"
	 "achieving a reported 99% on-time arrival rate.\n"
	 "  Independent (U.K.); Jasmine Fernández (June 8, 2026)\n\n"
	 "AI Degree Programs Surge\n"
	 "AI degree programs are rapidly expanding across U.S. colleges and\n"
	 "universities. The number of AI majors has grown from just five programs\n"
	 "in 2021 to at least 74 majors and 89 minors today, raising questions\n"
	 "about whether some programs represent innovation or simply rebranding.\n"
	 "  The New York Times; Alan Blinder (June 8, 2026)\n\n"
	 "Humanoid Robot to Run Convenience Store in Hong Kong\n"
	 "Hong Kong will launch its first 24/7 convenience store operated by a\n"
	 "humanoid robot, as part of a broader effort to promote public adoption\n"
	 "of AI. The robot-run capsule store will provide multilingual customer\n"
	 "service and sell a variety of products.\n"
	 "  Interesting Engineering; Atharva Gosavi (June 8, 2026)\n\n"
	 "OpenAI Files for IPO\n"
	 "OpenAI announced that it has filed a confidential S-1 for a potential\n"
	 "initial public offering, though no timeline has been set. The filing\n"
	 "comes one week after Anthropic submitted its own IPO paperwork. OpenAI\n"
	 "was valued at roughly $852 billion following a March funding round.\n"
	 "  NBC News; Steve Kopack (June 8, 2026)\n\n"
	 "Illinois Governor Hits Pause on Datacenter Subsidies\n"
	 "Illinois Governor JB Pritzker halted new state tax incentive agreements\n"
	 "for datacenters after lawmakers failed to approve a legislative pause.\n"
	 "Existing agreements remain in place, but new projects will not be\n"
	 "eligible for state support.\n"
	 "  Politico; Shia Kapos (June 5, 2026)\n"},

	{"[Thesis] Request for Thesis Check-in Meeting", "Clive Jarel Ang",
	 "06/12/2026(Fri) 4:50 PM", "3.12KB", true,
	 "Clive Jarel Ang <clive_jarel_c_ang@dlsu.edu.ph>",
	 "Jaimee Lannin <jaimee.lannin@adelaide.edu.au>",
	 "Fri, 12 Jun 2026 16:50:00 +0800",
	 "Cc: Justin Rainier Go, Roemer Gabriel Caliboso, Rinaldo Adelrico Lee,\n"
	 "    Thomas James Tiam-Lee\n\n"
	 "Dear Ms. Lannin,\n\n"
	 "I hope you and Dr. Abadia are doing well! Following the meeting with "
	 "Dr.\n"
	 "Abadia last month, we'd love to schedule our next thesis check-in at "
	 "her\n"
	 "earliest convenience.\n\n"
	 "Would any time within June work for her? We're generally free on\n"
	 "Wednesdays and the weekend but we're happy to arrange around her\n"
	 "schedule. Once you let us know her available windows, we'll confirm a\n"
	 "slot that suits everyone.\n\n"
	 "Thank you again for your time and assistance. We look forward to "
	 "hearing\n"
	 "from you!\n\n"
	 "Best regards,\n"
	 "Clive Ang\n"
	 "De La Salle University"},

	{"Recommended Watch - CPU Scheduling + FCFS: [1253_CSOPESY_S06] - "
	 "INTRODUCTION TO OPERATING SYSTEMS",
	 "CSOPESY (Canvas)", "06/12/2026(Fri) 6:39 PM", "8.91KB", true,
	 "[1253_CSOPESY_S06] - INTRODUCTION TO OPERATING SYSTEMS "
	 "<notifications@instructure.com>",
	 "justin_rainier_go@dlsu.edu.ph", "Fri, 12 Jun 2026 18:39:00 +0800",
	 "Since our next meeting will still be on Tues next week (Jun 16), I\n"
	 "highly recommend you watch this lecture about CPU Scheduling & FCFS to\n"
	 "help you work on your FCFS HW\n\n"
	 "https://drive.google.com/file/d/1kYxEjga_qqEivW-6YqsSp8QLvf-XGiof/view"
	 "?usp=sharing\n\n"
	 "We will still discuss this in our next lecture\n\n"
	 "Replies to this email will be posted as a reply to the announcement,\n"
	 "which will be seen by everyone in the course.\n\n"
	 "View announcement | Update your notification settings"},
};

const char *kSearchFields[] = {"Subject",  "From",	"To",
							   "Extended", "Mixed", "Tag"};

// Icon stacked above a label, with a hover
// highlight. Returns true when clicked.
bool ToolbarButton(const char *id, const char *icon, const char *label) {
	const float w = 66.0f;
	const float h = 52.0f;
	ImVec2 pos = ImGui::GetCursorScreenPos();
	bool pressed = ImGui::InvisibleButton(id, ImVec2(w, h));
	bool hovered = ImGui::IsItemHovered();
	bool active = ImGui::IsItemActive();

	ImDrawList *dl = ImGui::GetWindowDrawList();
	if (hovered || active) {
		ImU32 c = ImGui::GetColorU32(active ? ImGuiCol_ButtonActive
											: ImGuiCol_ButtonHovered);
		dl->AddRectFilled(pos, ImVec2(pos.x + w, pos.y + h), c, 3.0f);
	}

	ImU32 txt = ImGui::GetColorU32(ImGuiCol_Text);
	ImVec2 isz = ImGui::CalcTextSize(icon);
	dl->AddText(ImVec2(pos.x + (w - isz.x) * 0.5f, pos.y + 7.0f), txt, icon);
	ImVec2 lsz = ImGui::CalcTextSize(label);
	dl->AddText(ImVec2(pos.x + (w - lsz.x) * 0.5f, pos.y + h - lsz.y - 6.0f),
				txt, label);
	return pressed;
}

void ToolbarSeparator() {
	ImGui::SameLine(0.0f, 4.0f);
	ImVec2 p = ImGui::GetCursorScreenPos();
	float h = 52.0f;
	ImGui::GetWindowDrawList()->AddLine(ImVec2(p.x, p.y + 6.0f),
										ImVec2(p.x, p.y + h - 6.0f),
										ImGui::GetColorU32(ImGuiCol_Separator));
	ImGui::SameLine(0.0f, 5.0f);
}

} // namespace

Mail::Mail()
	: Window("Chron Mail (CMail)", ImGuiWindowFlags_NoCollapse |
							   ImGuiWindowFlags_NoSavedSettings |
							   ImGuiWindowFlags_MenuBar) {}

void Mail::drawMenuBar() {
	if (!ImGui::BeginMenuBar())
		return;

	if (ImGui::BeginMenu("File")) {
		ImGui::MenuItem("Add mailbox");
		ImGui::MenuItem("Import mbox file...");
		ImGui::Separator();
		ImGui::MenuItem("Save as...", "Ctrl+S");
		ImGui::MenuItem("Print...", "Ctrl+P");
		ImGui::Separator();
		if (ImGui::MenuItem("Exit", "Ctrl+Q"))
			m_open = false;
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {
		ImGui::MenuItem("Copy", "Ctrl+C");
		ImGui::MenuItem("Select all", "Ctrl+A");
		ImGui::MenuItem("Find in current message...", "Ctrl+F");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("View")) {
		ImGui::MenuItem("Show or hide", nullptr, false, false);
		ImGui::MenuItem("Set displayed columns");
		ImGui::MenuItem("Sort");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Message")) {
		ImGui::MenuItem("Receive");
		ImGui::MenuItem("Send queued messages");
		ImGui::MenuItem("Compose a new message", "Ctrl+M");
		ImGui::MenuItem("Reply", "Ctrl+R");
		ImGui::MenuItem("Forward", "Ctrl+L");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Tools")) {
		ImGui::MenuItem("Address book", "Ctrl+Shift+A");
		ImGui::MenuItem("Filter all messages in folder");
		ImGui::MenuItem("Actions");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Configuration")) {
		ImGui::MenuItem("Preferences for current account");
		ImGui::MenuItem("Create new account...");
		ImGui::MenuItem("Edit accounts...");
		ImGui::Separator();
		ImGui::MenuItem("Plugins...");
		ImGui::MenuItem("Preferences...");
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Help")) {
		ImGui::MenuItem("Manual");
		ImGui::MenuItem("About");
		ImGui::EndMenu();
	}
	ImGui::EndMenuBar();
}

void Mail::drawToolbar() {
	ImGui::BeginChild("##toolbar", ImVec2(0.0f, 58.0f), false,
					  ImGuiWindowFlags_NoScrollbar);
	ToolbarButton("##getmail", kIconGetMail, "Get Mail");
	ImGui::SameLine();
	ToolbarSeparator();
	ToolbarButton("##send", kIconSend, "Send");
	ImGui::SameLine();
	ToolbarButton("##compose", kIconCompose, "Compose");
	ImGui::SameLine();
	ToolbarSeparator();
	ToolbarButton("##reply", kIconReply, "Reply");
	ImGui::SameLine();
	ToolbarButton("##all", kIconReplyAll, "All");
	ImGui::SameLine();
	ToolbarButton("##sender", kIconSender, "Sender");
	ImGui::SameLine();
	ToolbarButton("##forward", kIconForward, "Forward");
	ImGui::SameLine();
	ToolbarSeparator();
	ToolbarButton("##wastebin", kIconTrash, "Wastebin");
	ImGui::SameLine();
	ToolbarButton("##spam", kIconSpam, "Spam");
	ImGui::EndChild();
	ImGui::Separator();
}

void Mail::drawFolderPane() {
	ImGui::BeginChild("##folders", ImVec2(220.0f, 0.0f), true);

	if (ImGui::BeginTable("##foldertable", 4,
						  ImGuiTableFlags_BordersInnerV |
							  ImGuiTableFlags_RowBg |
							  ImGuiTableFlags_ScrollY)) {
		ImGui::TableSetupColumn("Folder", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn(kIconEnvelope, ImGuiTableColumnFlags_WidthFixed,
								26.0f);
		ImGui::TableSetupColumn(kIconEnvelopeOpen,
								ImGuiTableColumnFlags_WidthFixed, 26.0f);
		ImGui::TableSetupColumn(kIconInbox, ImGuiTableColumnFlags_WidthFixed,
								26.0f);
		ImGui::TableHeadersRow();

		for (int i = 0; i < IM_ARRAYSIZE(kFolders); ++i) {
			const Folder &f = kFolders[i];
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);

			if (f.indent > 0)
				ImGui::Indent(16.0f * f.indent);

			char label[64];
			std::snprintf(label, sizeof(label), "%s %s##folder%d", f.icon,
						  f.name, i);
			bool isParent = (i == 0);
			bool selected = (!isParent && m_selectedFolder == i);
			if (ImGui::Selectable(label, selected,
								  ImGuiSelectableFlags_SpanAllColumns)) {
				if (!isParent)
					m_selectedFolder = i;
			}

			if (f.indent > 0)
				ImGui::Unindent(16.0f * f.indent);

			// Count columns; Chron shows a dash for the mailbox root.
			const char *fmt = isParent ? "-" : "%d";
			ImGui::TableSetColumnIndex(1);
			isParent ? ImGui::TextUnformatted("-")
					 : ImGui::Text(fmt, f.newCount);
			ImGui::TableSetColumnIndex(2);
			isParent ? ImGui::TextUnformatted("-") : ImGui::Text(fmt, f.unread);
			ImGui::TableSetColumnIndex(3);
			isParent ? ImGui::TextUnformatted("-") : ImGui::Text(fmt, f.total);
		}
		ImGui::EndTable();
	}

	ImGui::EndChild();
}

void Mail::drawMessageList() {
	if (ImGui::BeginTable("##msglist", 7,
						  ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg |
							  ImGuiTableFlags_Resizable |
							  ImGuiTableFlags_ScrollY |
							  ImGuiTableFlags_Reorderable)) {
		ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed,
								22.0f); // check
		ImGui::TableSetupColumn("S", ImGuiTableColumnFlags_WidthFixed, 22.0f);
		ImGui::TableSetupColumn(kIconPaperclip,
								ImGuiTableColumnFlags_WidthFixed, 22.0f);
		ImGui::TableSetupColumn("Subject", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("From", ImGuiTableColumnFlags_WidthFixed,
								150.0f);
		ImGui::TableSetupColumn("Date", ImGuiTableColumnFlags_WidthFixed,
								150.0f);
		ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed,
								60.0f);
		ImGui::TableHeadersRow();

		for (int i = 0; i < IM_ARRAYSIZE(kMessages); ++i) {
			const Message &m = kMessages[i];
			ImGui::TableNextRow();

			ImGui::TableSetColumnIndex(0);
			char sel[32];
			std::snprintf(sel, sizeof(sel), "##msg%d", i);
			if (ImGui::Selectable(sel, m_selectedMessage == i,
								  ImGuiSelectableFlags_SpanAllColumns))
				m_selectedMessage = i;

			ImGui::TableSetColumnIndex(1);
			ImGui::TextUnformatted(m.unread ? kIconEnvelope
											: kIconEnvelopeOpen);

			ImGui::TableSetColumnIndex(3);
			ImGui::TextUnformatted(m.subject);
			ImGui::TableSetColumnIndex(4);
			ImGui::TextUnformatted(m.from);
			ImGui::TableSetColumnIndex(5);
			ImGui::TextUnformatted(m.date);
			ImGui::TableSetColumnIndex(6);
			ImGui::TextUnformatted(m.size);
		}
		ImGui::EndTable();
	}
}

void Mail::drawPreviewPane() {
	const Message &m = kMessages[m_selectedMessage];

	ImGui::BeginChild("##preview", ImVec2(0.0f, 0.0f), true);

	ImU32 hdr = ImGui::GetColorU32(ImGuiCol_TextDisabled);
	auto headerLine = [&](const char *key, const char *val, ImVec4 valColor) {
		ImGui::TextColored(ImGui::ColorConvertU32ToFloat4(hdr), "%s", key);
		ImGui::SameLine();
		ImGui::TextColored(valColor, "%s", val);
	};

	ImVec4 green(0.20f, 0.55f, 0.25f, 1.0f);
	headerLine("From:", m.fromAddr, green);
	headerLine("To:", m.to, green);
	headerLine("Subject:", m.subject, ImGui::GetStyleColorVec4(ImGuiCol_Text));
	headerLine("Date:", m.dateFull, ImGui::GetStyleColorVec4(ImGuiCol_Text));

	ImGui::Separator();
	ImGui::Spacing();
	ImGui::TextUnformatted(m.body);

	ImGui::EndChild();
}

void Mail::drawStatusBar() {
	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::TextDisabled("%s Online", kIconOnline);

	const char *account = "justin_rainier_go@dlsu.edu.ph";
	float tw = ImGui::CalcTextSize(account).x;
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - tw -
						 ImGui::GetStyle().WindowPadding.x);
	ImGui::TextUnformatted(account);
}

void Mail::draw() {
	if (!m_open)
		return;

	ImGui::SetNextWindowSize(size(), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(position(), ImGuiCond_FirstUseEver);

	bool open = m_open;
	if (!ImGui::Begin(
			"Chron Mail (CMail) (Design inspired from Claws Mail)", &open,
			ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar |
				ImGuiWindowFlags_NoScrollWithMouse)) {
		ImGui::End();
		m_open = open;
		return;
	}

	drawMenuBar();
	drawToolbar();

	float footer = ImGui::GetFrameHeightWithSpacing();
	ImGui::BeginChild("##body", ImVec2(0.0f, -footer));

	drawFolderPane();
	ImGui::SameLine();

	ImGui::BeginGroup();
	float listHeight = ImGui::GetContentRegionAvail().y * 0.42f;

	// Message list.
	ImGui::BeginChild("##listpane", ImVec2(0.0f, listHeight), true);
	drawMessageList();
	ImGui::EndChild();

	// Quick-search / selection status row.
	int total = IM_ARRAYSIZE(kMessages);
	int unread = 0;
	for (const Message &msg : kMessages)
		unread += msg.unread ? 1 : 0;

	ImGui::TextDisabled("%s %s", kIconInbox, kFolders[m_selectedFolder].name);
	ImGui::SameLine();
	ImGui::Text("1 item selected (%s)", kMessages[m_selectedMessage].size);
	ImGui::SameLine();
	char counts[64];
	std::snprintf(counts, sizeof(counts), "0 new, %d unread, %d total", unread,
				  total);
	float cw = ImGui::CalcTextSize(counts).x;
	ImGui::SameLine();
	ImGui::SetCursorPosX(ImGui::GetCursorPosX() +
						 ImGui::GetContentRegionAvail().x - cw);
	ImGui::TextDisabled("%s", counts);

	ImGui::TextUnformatted(kIconSearch);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100.0f);
	ImGui::Combo("##searchfield", &m_searchField, kSearchFields,
				 IM_ARRAYSIZE(kSearchFields));
	ImGui::SameLine();
	ImGui::SetNextItemWidth(220.0f);
	ImGui::InputText("##searchbox", m_searchBuf, sizeof(m_searchBuf));
	ImGui::SameLine();
	if (ImGui::Button("Clear"))
		m_searchBuf[0] = '\0';

	ImGui::Spacing();

	// Preview pane.
	drawPreviewPane();
	ImGui::EndGroup();

	ImGui::EndChild();

	drawStatusBar();

	ImGui::End();
	m_open = open;
}

ImVec2 Mail::position() {
	ImGuiViewport *viewport = ImGui::GetMainViewport();
	ImVec2 sz = size();
	return ImVec2(viewport->WorkPos.x + (viewport->WorkSize.x - sz.x) * 0.5f,
				  viewport->WorkPos.y + (viewport->WorkSize.y - sz.y) * 0.5f);
}

ImVec2 Mail::size() { return ImVec2(900.0f, 640.0f); }
