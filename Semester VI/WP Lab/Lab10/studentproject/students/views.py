from django.shortcuts import render, redirect
from .forms import StudentForm
from .models import Student

def add_student(request):
    if request.method == 'POST':
        form = StudentForm(request.POST)
        if form.is_valid():
            form.save()
            return redirect('index')
    else:
        form = StudentForm()
    return render(request, 'add_student.html', {'form': form})

def index(request):
    students = Student.objects.all()
    return render(request, 'index.html', {'students': students})
