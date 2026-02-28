from django.shortcuts import render, redirect
from .forms import StudentForm

def first_page(request):
    if request.method == 'POST':
        form = StudentForm(request.POST)
        if form.is_valid():
            request.session['name'] = form.cleaned_data['name']
            request.session['roll_no'] = form.cleaned_data['roll_no']
            request.session['subject'] = form.cleaned_data['subject']
            return redirect('details_show')
    else:
        form = StudentForm()
    return render(request, 'firstPage.html', {'form': form})

def second_page(request):
    return render(request, 'secondPage.html')