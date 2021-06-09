class Course
	string course_name;
	int credits;
	string pre_courses;
	string grade;
endclass

func void ctor(Course c, string c_n, int cre, string p_c, string g)
    c.course_name = c_n;
    c.credits = cre;
    c.pre_courses = p_c;
    c.grade = g;
endfunc

func void read_Course(Course c)
	string t1 := "False";
	string t2 := "";
    ctor(c, t1, 0, t2, t2);
	
	string ch := readChar();
	string str := "";
	string zero := "0";
	int res := 0;
	int cnt := 0;
	while (ch != "\n")
		if (ch == "|")
			if (cnt == 0)
				c.course_name = str;
				str = "";
			endif
			if (cnt == 1)
				printString("");
				c.credits = res;
			endif
			if (cnt == 2)
				printString("");
				printString("");
				c.pre_courses = str;
				str = "";
			endif
			cnt = cnt + 1;
		endif
		if (ch != "|")
			printString("");
			if (cnt == 0 || cnt == 2)

				str = str + ch;
			endif
			if (cnt == 1)
				res = res * 10 + (ch - zero);
			endif
			if (cnt == 3)
				printString("");
				c.grade = ch;
			endif
		endif
		ch = readChar();
	endwhile
	printString("");

	return c;
endfunc

func void read_Courses(Course[] course_list)
	int i := 0;
	course_list[0] = allocate(4);
	read_Course(course_list[0]);
	while (course_list[i].course_name != "False")
		i = i + 1;
		course_list[i] = allocate(4);
		read_Course(course_list[i]);
	endwhile
endfunc

func int check_course(string course_name, Course[] course_list)
	if (course_name == "")
		return 1;
	endif
	int i := 0;
	while (course_list[i].course_name != "False")
		string cn := course_list[i].course_name;
		string cg := course_list[i].grade;
		string empty := "";
		if (course_name == cn && cg != empty && cg != "F")
			return 1;
		endif
		i = i + 1;
	endwhile
	return 0;
endfunc

func int is_possible_course(string pre_courses_str, Course[] course_list)
	string str := "";
	int i := 0;
	int res := 0;
	int tmp_res := 1;
	int t1;
	int t2;
	while (stringAt(pre_courses_str, i) != "")
		t1 = stringAt(pre_courses_str, i) == ";";
		t2 = stringAt(pre_courses_str, i) == ",";
		if (t1)
			tmp_res = tmp_res && check_course(str, course_list);
			str = "";
			res = res || tmp_res;
			tmp_res = 1;
		endif
		if (t2)
			tmp_res = tmp_res && check_course(str, course_list);
			str = "";
		endif
		if (t1 == 0 && t2 == 0)
			str = str + stringAt(pre_courses_str, i);
		endif
		i = i + 1;
	endwhile
	return (tmp_res && check_course(str, course_list)) || res;
endfunc

func void parse_Course(Course[] course_list)
	int i := 0;
	double grades := 0.0;
	int hours_attempted := 0;
	int hours_completed := 0;
	int credits_remaining := 0;
	string courses_to_take[] := allocate(500);
	int ctt_cnt := 0;
	int t1;
	int t2;
	int t3;
	while (course_list[i].course_name != "False")

		t1 = course_list[i].grade == "";
		t2 = course_list[i].grade == "F";
		if (t1)
			credits_remaining = credits_remaining + course_list[i].credits;
			if (is_possible_course(course_list[i].pre_courses, course_list))
				courses_to_take[ctt_cnt] = course_list[i].course_name;
				ctt_cnt = ctt_cnt + 1;
			endif
		endif
		if (t2)
			credits_remaining = credits_remaining + course_list[i].credits;
			hours_attempted = hours_attempted + course_list[i].credits;
			if (is_possible_course(course_list[i].pre_courses, course_list))
				courses_to_take[ctt_cnt] = course_list[i].course_name;
				ctt_cnt = ctt_cnt + 1;
			endif
		endif
		if (t1 == 0 && t2 == 0)

			hours_attempted = hours_attempted + course_list[i].credits;
			hours_completed = hours_completed + course_list[i].credits;
			t3 = ("E" - course_list[i].grade) * course_list[i].credits;

			grades = grades + t3;

		endif
		i = i + 1;
	endwhile
	printString("GPA: ");
	if (hours_attempted == 0)
		printDouble(grades, 1);
	else 
		printDouble(grades / hours_attempted, 1);
	endif
	printString("\nHours Attempted: ");
	printInt(hours_attempted, 0);
	printString("\nHours Completed: ");
	printInt(hours_completed, 0);
	printString("\nCredits Remaining: ");
	printInt(credits_remaining, 0);
	printString("\n\nPossible Courses to Take Next\n");
	i = 0;
    if (credits_remaining == 0 && ctt_cnt == 0)
        printString("  None - Congratulations!\n");
	endif

	while (i < ctt_cnt)
		string temp := "  " + courses_to_take[i];
		printString(temp);
		printString("\n");
		i = i + 1;
	endwhile



endfunc


func int main()
	Course course_list[] := allocate(400);
	read_Courses(course_list);
	parse_Course(course_list);
	return 0;
endfunc