# carapp/views.py
from django.shortcuts import render, redirect
from .forms import CarSearchForm

def input(request):
    if request.method == 'POST':
        form = CarSearchForm(request.POST)
        if form.is_valid():
            make = form.cleaned_data['manufacturer']
            model = form.cleaned_data['model_name']
            return render(request, 'result.html', {'make': make, 'model': model})
    else:
        form = CarSearchForm()
    
    return render(request, 'input.html', {'form': form})