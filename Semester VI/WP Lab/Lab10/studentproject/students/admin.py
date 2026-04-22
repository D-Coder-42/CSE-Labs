from django.contrib import admin
from .models import Student

@admin.register(Student)
class StudentAdmin(admin.ModelAdmin):
    list_display = ('student_id', 'student_name', 'course_name', 'date_of_birth')
    search_fields = ('student_id', 'student_name', 'course_name')
