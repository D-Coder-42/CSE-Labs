from django.shortcuts import render
from .models import Works, Lives
from .forms import WorksForm, CompanySearchForm

def insert_works(request):
    if request.method == 'POST':
        form = WorksForm(request.POST)
        if form.is_valid():
            form.save()
            return render(request, 'insert_success.html', {'form': form})
    else:
        form = WorksForm()
    return render(request, 'insert_works.html', {'form': form})

def search_company(request):
    people = None
    company = None
    if request.method == 'POST':
        form = CompanySearchForm(request.POST)
        if form.is_valid():
            company = form.cleaned_data['company_name']
            works = Works.objects.filter(company_name=company)
            people = []
            for w in works:
                try:
                    city = Lives.objects.get(person_name=w.person_name).city
                except Lives.DoesNotExist:
                    city = "Unknown"
                people.append((w.person_name, city))
    else:
        form = CompanySearchForm()
    return render(request, 'search_company.html', {'form': form, 'company': company, 'people': people})
